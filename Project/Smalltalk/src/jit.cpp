#include "jit.h"
#include <iostream>

JIT::JIT(int threshold) : jitThreshold(threshold) {
  std::cout << "JIT constructor" << std::endl;
  JIT::basicBlockList = std::vector<BasicBlock *>();
  return;
}

void JIT::startBasicBlock(std::string start) {
  /* std::cout << "Looking for Location: " << start << std::endl; */
  /* if (this->basicBlocks.find(start) != this->basicBlocks.end()) { */

  auto foo = true;

  // find the basic block in the list

  for (auto &bb : this->basicBlockList) {
    if (bb->start == start) {
      /* std::cout << "Basic block at " << start << " already exists " */
      /* << bb->blockId << std::endl; */
      foo = false;

      bb->heat++;

      /* std::cout << "Basic block already exists: " << existing->toString() */
      /*           << std::endl; */

      setCurrentBasicBlock(bb);

      if (bb->heat >= JIT::jitThreshold && bb->compiled == false) {
        bb->compiled = true;
        if (!(bb->instructions[0].bytecode >= 144) &&
            (bb->instructions[0].bytecode <= 175)) {
          // the block starts and therefore also ends with a jump instruction
          // no need to compile that
          /* std::cout << "Compiling basic block: " << bb->toString() <<
           * std::endl; */
          // TODO: compile the basic block
          // JIT::compileBasicBlock(existing);
        }
      }
      return;
    }
  }

  if (foo) {
    JIT::blockID++;
    /* std::cout << "Starting new basic block " << JIT::blockID << " at " <<
     * start */
    /*           << std::endl; */
    BasicBlock *bb = new BasicBlock(JIT::blockID, start);
    /* bb->start = start; */
    JIT::setCurrentBasicBlock(bb);
  }
}

void JIT::endBasicBlock(std::string current, std::string next) {
  if (!currentBasicBlock->hasEnded) {
    /* std::cout << "Ending basicblock " << currentBasicBlock->blockId << " with
     * " */
    /*           << currentBasicBlock->instructions.size() << " instructions" */
    /*           << std::endl; */

    this->currentBasicBlock->end = current;
    this->currentBasicBlock->next = next;
    this->currentBasicBlock->hasEnded = true;
    /* this->basicBlocks[currentBasicBlock->start] = currentBasicBlock; */
    /* this->basicBlocks.insert(std::pair<Location *, BasicBlock *>( */
    /*     currentBasicBlock->start, currentBasicBlock)); */
    this->basicBlockList.push_back(currentBasicBlock);
    /* std::cout << "BasicBlocks has now " << this->basicBlockList.size() */
    /*           << " elements" << std::endl; */
  }
  JIT::startBasicBlock(next);
}
