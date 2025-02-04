#pragma once

#include <iomanip>
#include <keystone/keystone.h>
#include <map>
#include <string>
#include <vector>

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

  Location(int active, int method, int instr) {
    ActiveContext = active;
    Method = method;
    InstructionPointer = instr;
  }

  Location offsetLocation(int offset) {
    this->InstructionPointer = this->InstructionPointer + offset;
    return *this;
  }
};

struct Instruction {
  int bytecode;
  std::string location;
  std::string name;

  std::string toString() {
    std::stringstream stream;
    stream << std::setw(3) << std::to_string(bytecode) << "  " << name;
    return stream.str();
  }
};

struct BasicBlock {
  int blockId;
  std::string start;
  std::string end;
  bool hasEnded;
  int heat;
  bool compiled;
  std::string next;
  std::vector<Instruction> instructions;

  std::string toString() {
    std::string str = "BasicBlock: " + std::to_string(blockId) +
                      "\n   Start: " + start + "\n   End: " + end +
                      "\n   Heat: " + std::to_string(heat) +
                      "\n   next: " + next + "\n   Instructions (" +
                      std::to_string(instructions.size()) + "):";
    for (size_t i = 0; i < instructions.size(); i++) {
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
  BasicBlock *currentBasicBlock;
  std::map<std::string, BasicBlock *> basicBlocks{};
  std::vector<BasicBlock *> basicBlockList{};

  JIT(int threshold);
  void startBasicBlock(std::string start);
  void endBasicBlock(std::string currentLocation, std::string nextLocation);
  void compileBasicBlock(BasicBlock *bb);
  std::string compileToMC(std::string inputASM);
  void translateInstruction(Instruction inst);
  bool isContextSwitchingInstruction(int bc);
  std::string getInstructionDescription(int bytecode);

  ks_engine *ks;
  ks_err ksErr;
  size_t ksCount;
  unsigned char *ksEncode;
  size_t ksSize;

private:
  int jitThreshold = 1000;
  int blockID = 0;

  void setCurrentBasicBlock(BasicBlock *cbb) { currentBasicBlock = cbb; }
};
