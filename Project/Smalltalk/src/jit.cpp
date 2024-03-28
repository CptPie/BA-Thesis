#include "jit.h"
#include <iostream>

JIT::JIT(int threshold) : jitThreshold(threshold) {
  std::cout << "JIT constructor" << std::endl;
  return;
}

void JIT::startBasicBlock(Location *start) {
  std::cout << "Looking for Location: " << start->toString() << std::endl;
  if (!basicBlocks.empty()) {
    std::cout << "BasicBlocks has these entries: " << std::endl;
    for (std::map<Location *, BasicBlock *>::iterator it = basicBlocks.begin();
         it != basicBlocks.end(); ++it) {
      std::cout << "  " << it->first->toString() << std::endl;
    }
  }

  if (this->basicBlocks.find(start) != this->basicBlocks.end()) {
    BasicBlock *existing = JIT::basicBlocks[start];
    std::cout << "Basic block at " << start->toString()
              << " already exists with id " << existing->blockId << std::endl;
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
              << start->toString() << std::endl;
    BasicBlock *bb = new BasicBlock(JIT::blockID, start);
    JIT::setCurrentBasicBlock(bb);
  }
}

void JIT::endBasicBlock(Location *current, Location *next) {
  std::cout << "Ending basicblock " << currentBasicBlock->blockId
            << "\n Current: " << current->toString()
            << "\n Instructions: " << currentBasicBlock->instructions.size()
            << "\n Next: " << next->toString() << std::endl;

  this->currentBasicBlock->end = current;
  this->currentBasicBlock->next = next;
  this->currentBasicBlock->hasEnded = true;
  /* this->basicBlocks[currentBasicBlock->start] = currentBasicBlock; */
  this->basicBlocks.insert(std::pair<Location *, BasicBlock *>(
      currentBasicBlock->start, currentBasicBlock));
  std::cout << "BasicBlocks has now " << this->basicBlocks.size() << " elements"
            << std::endl;

  JIT::startBasicBlock(next);
}
