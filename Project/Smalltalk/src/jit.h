#pragma once

#include <iomanip>
#include <map>
#include <string>
#include <vector>

struct Instruction {
  int bytecode;
  int location; // program counter
  std::string Name;

  std::string toString() {
    std::stringstream stream;
    stream << std::setw(3) << std::to_string(bytecode) << "  " << Name;
    return stream.str();
  }
};

struct BasicBlock {
  int blockId;     // unique id for the basic block
  int start;       // start IC of the basic block
  int end;         // end IC of the basic block
  bool hasEnded;   // has the block ended
  int heat;        // number of times the block has been executed
  bool compiled;   // has the block been compiled
  int nextAddress; // the next address after the block
  std::vector<Instruction> instructions; // the instructions in the basic block

  std::string toString() {
    std::string str = "BasicBlock: " + std::to_string(blockId) +
                      "\n   Start: " + std::to_string(start) +
                      "\n   End: " + std::to_string(end) +
                      "\n   Heat: " + std::to_string(heat) +
                      "\n   nextAddress: " + std::to_string(nextAddress) +
                      "\n   Instructions (" +
                      std::to_string(instructions.size()) + "):";
    for (int i = 0; i < instructions.size(); i++) {
      str += "\n      " + instructions[i].toString();
    }
    str += "\nEnd BasicBlock\n";
    return str;
  }

  BasicBlock(int blockID, int start) {
    blockId = blockID;
    start = start;
    end = 0;
    heat = 1;
    compiled = false;
    hasEnded = false;
    instructions = std::vector<Instruction>();
  }
};

class JIT {

public:
  BasicBlock *currentBasicBlock{};
  std::map<int, BasicBlock *> basicBlocks{};

  JIT(int threshold);
  void startBasicBlock(int startPC); // start a new basic block
  void endBasicBlock(int currentPC,
                     int nextAddress); // end the current basic block

private:
  int jitThreshold = 100; // default
  int blockID = 0;

  void setCurrentBasicBlock(BasicBlock *cbb) { currentBasicBlock = cbb; }
};
