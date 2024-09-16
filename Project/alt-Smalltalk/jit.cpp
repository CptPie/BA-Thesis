#include "jit.h"
#include "keystone/keystone.h"
#include <iostream>

JIT::JIT(int threshold) : jitThreshold(threshold) {
  JIT::basicBlocks = std::map<Location *, BasicBlock *>();

  ks_engine *ks;

  JIT::ksErr = ks_open(KS_ARCH_RISCV, KS_MODE_RISCV64, &ks);

  JIT::ks = ks;

  if (JIT::ksErr != KS_ERR_OK) {
    printf("ERROR: failed on ks_open(), terminating\n");
    exit(-1);
  }

  return;
}

void JIT::startBasicBlock(Location *start) {
  if (basicBlocks.find(start) != this->basicBlocks.end()) {
    // This basicBlock already exists, raise its heat
    BasicBlock *bb = basicBlocks[start];

    bb->heat++;

    setCurrentBasicBlock(bb);

    // the Block is a "hot" block, compile it if it hasn't already
    if (bb->heat > JIT::jitThreshold && bb->compiled == false) {

      bb->compiled = true;

      // if the BasicBlock starts with a contextSwitch we can skip compiling
      // since its just a handover
      if (!JIT::isContextSwitchingInstruction(bb->instructions[0].bytecode)) {
        JIT::compileBasicBlock(bb);
      }
    }
    return;
  }
  JIT::blockID++;

  BasicBlock *bb = new BasicBlock(JIT::blockID, start);
  JIT::setCurrentBasicBlock(bb);
}

std::string JIT::getInstructionDescription(int bytecode) {
  switch (bytecode) {
  case 0 ... 15:
    return "Push Receiver Variable";
  case 16 ... 31:
    return "Push Temporary Location";
  case 32 ... 63:
    return "Push Literal Constant";
  case 64 ... 95:
    return "Push Literal Variable";
  case 96 ... 103:
    return "Pop and Store Receiver Variable";
  case 104 ... 111:
    return "Pop and Store Temporary Location";
  case 112 ... 119:
    return "Push";
  case 120 ... 123:
    return "Return";
  case 124 ... 125:
    return "Return Stack Top";
  case 126 ... 127:
    return "unused";
  case 128:
    return "Push";
  case 129:
    return "Store";
  case 130:
    return "Pop and Store";
  case 131:
    return "Send Literal 5 bit Selector with 3 bit Arguments";
  case 132:
    return "Send Literal 8 bit Selector with 8 bit Arguments";
  case 133:
    return "Send Literal Selector 5 bit to Superclass with 3 bit Arguments";
  case 134:
    return "Send Literal Selector 8 bit to Superclass with 8 bit Arguments";
  case 135:
    return "Pop Stack Top";
  case 136:
    return "Duplicate Stack Top";
  case 137:
    return "Push Active Context";
  case 138 ... 143:
    return "unused";
  case 144 ... 151:
    return "Jump Short";
  case 152 ... 159:
    return "Pop and Jump Short on False";
  case 160 ... 167:
    return "Jump Long";
  case 168 ... 171:
    return "Pop and Jump Long on True";
  case 172 ... 175:
    return "Pop and Jump Long on False";
  case 176 ... 191:
    return "Send Arithmetic Message";
  case 192 ... 207:
    return "Send Special Message";
  case 208 ... 223:
    return "Send Literal Selector with 0 Arguments";
  case 224 ... 239:
    return "Send Literal Selector with 1 Argument";
  case 240 ... 255:
    return "Send Literal Selector with 2 Arguments";
  default:
    return "unknown";
  }
}

// TODO: investigate if the sends should also be added here
bool JIT::isContextSwitchingInstruction(int bc) {
  return ((bc >= 120 && bc <= 125)    // Return statements
          || (bc >= 144 && bc <= 175) // (Pop and) Jumps
  );
}

void JIT::compileBasicBlock(BasicBlock *bb) {
  for (Instruction inst : bb->instructions) {
    translateInstruction(inst);
  }
}

void JIT::translateInstruction(Instruction inst) {
  int bc = inst.bytecode;
  if (!isContextSwitchingInstruction(bc)) {
    std::cerr << "Compiling BC: " << bc << getInstructionDescription(bc);
  }
}

void JIT::endBasicBlock(Location *current, Location *next) {
  if (!currentBasicBlock->hasEnded) {
    this->currentBasicBlock->end = current;
    this->currentBasicBlock->next = next;
    this->currentBasicBlock->hasEnded = true;
    this->basicBlocks[currentBasicBlock->start] = currentBasicBlock;
  }
  JIT::startBasicBlock(next);
}
