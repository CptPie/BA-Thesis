#include "jit.h"
#include <iostream>
#include <map>

JIT::JIT(int threshold) : jitThreshold(threshold) {
  std::cout << "JIT constructor" << std::endl;
  /* JIT::basicBlockList = std::vector<BasicBlock *>(); */
  JIT::basicBlocks = std::map<std::string, BasicBlock *>();
  return;
}

void JIT::startBasicBlock(std::string start) {
  std::cout << "Looking for Location: " << start;

  if (basicBlocks.find(start) != this->basicBlocks.end()) {

    BasicBlock *bb = basicBlocks[start];

    std::cout << " found " << bb->blockId << std::endl;

    bb->heat++;

    setCurrentBasicBlock(bb);

    if (bb->heat >= JIT::jitThreshold && bb->compiled == false) {
      bb->compiled = true;
      if (!(bb->instructions[0].bytecode >= 144) &&
          (bb->instructions[0].bytecode <= 175)) {
        // the block starts and therefore also ends with a jump instruction
        // no need to compile that
        std::cout << "Compiling basic block: " << bb->toString() << std::endl;
        // TODO: compile the basic block
        // JIT::compileBasicBlock(existing);
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
  std::cout << std::endl
            << "Starting new basic block " << JIT::blockID << " at " << start
            << std::endl;
  BasicBlock *bb = new BasicBlock(JIT::blockID, start);
  /* bb->start = start; */
  JIT::setCurrentBasicBlock(bb);
}

void JIT::endBasicBlock(std::string current, std::string next) {
  if (!currentBasicBlock->hasEnded) {
    std::cout << "Ending basicblock " << currentBasicBlock->blockId << " with "
              << currentBasicBlock->instructions.size() << " instructions"
              << std::endl;

    this->currentBasicBlock->end = current;
    this->currentBasicBlock->next = next;
    this->currentBasicBlock->hasEnded = true;
    this->basicBlocks[currentBasicBlock->start] = currentBasicBlock;
    /* this->basicBlockList.push_back(currentBasicBlock); */
    std::cout << "BasicBlocks has now " << this->basicBlocks.size()
              << " elements" << std::endl;
  }
  JIT::startBasicBlock(next);
}
