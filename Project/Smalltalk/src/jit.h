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

struct Location {
  int ActiveContext;
  int Method;
  int InstructionPointer;

  std::string toString() {
    return std::to_string(ActiveContext) + ":" + std::to_string(Method) + ":" +
           std::to_string(InstructionPointer);
  }

  bool operator==(const Location &other) const {
    return ActiveContext == other.ActiveContext && Method == other.Method &&
           InstructionPointer == other.InstructionPointer;
  }
};

struct BasicBlock {
  int blockId;       // unique id for the basic block
  std::string start; // start IC of the basic block
  std::string end;   // end IC of the basic block
  bool hasEnded;     // has the block ended
  int heat;          // number of times the block has been executed
  bool compiled;     // has the block been compiled
  std::string next;  // the next address after the block
  std::vector<Instruction> instructions; // the instructions in the basic block

  std::string toString() {
    std::string str = "BasicBlock: " + std::to_string(blockId) +
                      "\n   Start: " + start + "\n   End: " + end +
                      "\n   Heat: " + std::to_string(heat) +
                      "\n   next: " + next + "\n   Instructions (" +
                      std::to_string(instructions.size()) + "):";
    for (int i = 0; i < instructions.size(); i++) {
      str += "\n      " + instructions[i].toString();
    }
    str += "\nEnd BasicBlock\n";
    return str;
  }

  BasicBlock(int blockID, std::string startLocation) {
    blockId = blockID;
    start = startLocation;
    end = "";
    heat = 1;
    compiled = false;
    hasEnded = false;
    instructions = std::vector<Instruction>();
  }
};

class JIT {

public:
  BasicBlock *currentBasicBlock{};
  std::map<std::string, BasicBlock *> basicBlocks{};
  std::vector<BasicBlock *> basicBlockList{}; // list of basic blocks

  JIT(int threshold);
  void startBasicBlock(std::string start); // start a new basic block
  void endBasicBlock(std::string current,
                     std::string next); // end the current basic block
  void compileBasicBlock(BasicBlock *block);
  void translateInstruction(Instruction inst);
  bool isContextSwitchingInstruction(int bc);

private:
  int jitThreshold = 1000; // default
  int blockID = 0;

  void setCurrentBasicBlock(BasicBlock *cbb) { currentBasicBlock = cbb; }
};
