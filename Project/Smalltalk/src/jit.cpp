#include "jit.h"
#include <iostream>

JIT::JIT(int threshold) : jitThreshold(threshold) {
  std::cout << "JIT constructor" << std::endl;
  return;
}

void JIT::startBasicBlock(int startPC) {
  /* std::cout << "Starting basic block at " << startPC << std::endl; */
  if (this->basicBlocks.find(startPC) != this->basicBlocks.end()) {
    BasicBlock *existing = JIT::basicBlocks[startPC];
    existing->heat++;

    /* std::cout << "Basic block already exists: " << existing->toString() */
    /*           << std::endl; */

    JIT::setCurrentBasicBlock(existing);

    if (existing->heat >= JIT::jitThreshold && existing->compiled == false) {
      existing->compiled = true;
      std::cout << "Compiling basic block: " << existing->toString()
                << std::endl;
      // TODO: compile the basic block
      // JIT::compileBasicBlock(existing);
    }
    return;
  }

  JIT::blockID++;
  BasicBlock *bb = new BasicBlock(JIT::blockID, startPC);
  JIT::setCurrentBasicBlock(bb);
}

void JIT::endBasicBlock(int currentPC, int nextAddress) {
  /* std::cout << "Ending basicblock " << currentBasicBlock->blockId <<
   * std::endl; */

  this->currentBasicBlock->end = currentPC;
  this->currentBasicBlock->nextAddress = nextAddress;
  this->currentBasicBlock->hasEnded = true;
  this->basicBlocks[currentBasicBlock->start] = currentBasicBlock;

  JIT::startBasicBlock(nextAddress);
}
