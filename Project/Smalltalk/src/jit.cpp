#include "jit.h"
#include <iostream>
#include <map>

#ifdef DEBUG
#include <iostream>
#endif // DEBUG

JIT::JIT(int threshold) : jitThreshold(threshold) {
#ifdef DEBUG
  std::cout << "JIT constructor" << std::endl;
#endif // DEBUG
  /* JIT::basicBlockList = std::vector<BasicBlock *>(); */
  JIT::basicBlocks = std::map<std::string, BasicBlock *>();
  return;
}

void JIT::startBasicBlock(std::string start) {
#ifdef DEBUG
  std::cout << "Looking for Location: " << start;
#endif // DEBUG

  if (basicBlocks.find(start) != this->basicBlocks.end()) {

    BasicBlock *bb = basicBlocks[start];

#ifdef DEBUG
    std::cout << " found " << bb->blockId << std::endl;
#endif // DEBUG

    bb->heat++;

    setCurrentBasicBlock(bb);

    if (bb->heat >= JIT::jitThreshold && bb->compiled == false) {
      bb->compiled = true;
      if (!JIT::isContextSwitchingInstruction(bb->instructions[0].bytecode)) {
// the block starts and therefore also ends with a jump/return instruction
// no need to compile that
#ifdef DEBUG
        std::cout << "Compiling basic block: " << bb->toString() << std::endl;
#endif                              // DEBUG
        JIT::compileBasicBlock(bb); // keystone
      }
    }
    return;
  }

  /* for (auto &bb : this->basicBlockList) { */
  /*   if (bb->start == start) { */
  /*     std::cout << "Basic block at " << start << " already exists " */
  /*      << bb->blockId << std::endl; */
  /*     foo = false; */
  /**/
  /*     bb->heat++; */
  /**/
  /*      std::cout << "Basic block already exists: " << existing->toString() */
  /*                << std::endl; */
  /**/
  /*     setCurrentBasicBlock(bb); */
  /**/
  /*     if (bb->heat >= JIT::jitThreshold && bb->compiled == false) { */
  /*       bb->compiled = true; */
  /*       if (!(bb->instructions[0].bytecode >= 144) && */
  /*           (bb->instructions[0].bytecode <= 175)) { */
  /*         // the block starts and therefore also ends with a jump instruction
   */
  /*         // no need to compile that */
  /*         std::cout << "Compiling basic block: " << bb->toString() << */
  /*          * std::endl; */
  /*         // TODO: compile the basic block */
  /*         // JIT::compileBasicBlock(existing); */
  /*       } */
  /*     } */
  /*     return; */
  /*   } */
  /* } */

  JIT::blockID++;

#ifdef DEBUG
  std::cout << std::endl
            << "Starting new basic block " << JIT::blockID << " at " << start
            << std::endl;
#endif // DEBUG

  BasicBlock *bb = new BasicBlock(JIT::blockID, start);
  /* bb->start = start; */
  JIT::setCurrentBasicBlock(bb);
}

void JIT::endBasicBlock(std::string current, std::string next) {
  if (!currentBasicBlock->hasEnded) {
#ifdef DEBUG
    std::cout << "Ending basicblock " << currentBasicBlock->blockId << " with "
              << currentBasicBlock->instructions.size() << " instructions"
              << std::endl;
#endif // DEBUG

    this->currentBasicBlock->end = current;
    this->currentBasicBlock->next = next;
    this->currentBasicBlock->hasEnded = true;
    this->basicBlocks[currentBasicBlock->start] = currentBasicBlock;
#ifdef DEBUG
    std::cout << "BasicBlocks has now " << this->basicBlocks.size()
              << " elements" << std::endl;
#endif // DEBUG
  }
  JIT::startBasicBlock(next);
}

void JIT::compileBasicBlock(BasicBlock *block) {
  for (Instruction inst : block->instructions) {
    translateInstruction(inst);
  }
}

void JIT::translateInstruction(Instruction inst) {
  int bc = inst.bytecode;
  if (!isContextSwitchingInstruction(bc)) {
    std::cout << "Compiling BC " << bc << std::endl;
    switch (bc) {
      // TODO: here be magic
    }
  }
}

bool JIT::isContextSwitchingInstruction(int bc) {
  return ((bc >= 120 && bc <= 125) || (bc >= 144 && bc <= 175));
}
