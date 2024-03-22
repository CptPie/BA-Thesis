#include "jit.h"
#include <iostream>

JIT::JIT(int threshold) : jitThreshold(threshold) {
  std::cout << "JIT constructor" << std::endl;
  return;
}

void JIT::startBasicBlock(int startPC) {
  if (this->basicBlocks.find(startPC) != this->basicBlocks.end()) {
    BasicBlock *existing = JIT::basicBlocks[startPC];
    std::cout << "Basic block at " << startPC << " already exists with id "
              << existing->blockId << std::endl;
    existing->heat++;

    /* std::cout << "Basic block already exists: " << existing->toString() */
    /*           << std::endl; */

    setCurrentBasicBlock(existing);

    if (existing->heat >= JIT::jitThreshold && existing->compiled == false) {
      existing->compiled = true;
      std::cout << "Compiling basic block: " << existing->toString()
                << std::endl;
      // TODO: compile the basic block
      // JIT::compileBasicBlock(existing);
    }
  } else {
    JIT::blockID++;
    std::cout << "Starting new basic block " << JIT::blockID << " at "
              << startPC << std::endl;
    BasicBlock *bb = new BasicBlock(JIT::blockID, startPC);
    JIT::setCurrentBasicBlock(bb);
  }
}

void JIT::endBasicBlock(int currentPC, int nextAddress) {
  std::cout << "Ending basicblock " << currentBasicBlock->blockId
            << "\n CurrentIC: " << currentPC
            << "\n Instructions: " << currentBasicBlock->instructions.size()
            << "\n Next: " << nextAddress << std::endl;

  this->currentBasicBlock->end = currentPC;
  this->currentBasicBlock->nextAddress = nextAddress;
  this->currentBasicBlock->hasEnded = true;
  /* this->basicBlocks[currentBasicBlock->start] = currentBasicBlock; */
  this->basicBlocks.insert(std::pair<int, BasicBlock *>(
      currentBasicBlock->start, currentBasicBlock));
  std::cout << "BasicBlocks has now " << this->basicBlocks.size() << " elements"
            << std::endl;

  JIT::startBasicBlock(nextAddress);
}
