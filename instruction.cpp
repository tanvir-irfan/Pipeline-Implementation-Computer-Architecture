#include <cassert>
#include <iostream>
#include <string>
#include "instruction.h"

using namespace std;

void Instruction::init(InstructionType _type, RegisterType _arg1, RegisterType _arg2, RegisterType _arg3,
    int _immidiate, int _address) {
    type = _type;
    arg1 = _arg1;
    arg2 = _arg2;
    arg3 = _arg3;
    immidiate = _immidiate;
    address = _address;
    A = B = 0;
    aluOut = 0;
    loadMemData = 0;
    fetchedAtCycle = 0;
    srcCycle1 = 0;
    srcCycle2 = 0;
    branch_pc = 0;
    stalled = false;
}

Instruction::Instruction() {
    init(NOP, R0, R0, R0, 0, 0);
}

Instruction::Instruction(InstructionType _type, int _address) {
    init(_type, R0, R0, R0, 0, _address);
}

Instruction::Instruction(InstructionType _type, RegisterType _arg1, int _immidiate, int _address) {
    init(_type, _arg1, R0, R0, _immidiate, _address);
}

Instruction::Instruction(InstructionType _type, RegisterType _arg1, RegisterType _arg2,
    int _immidiate, int _address) {
    init(_type, _arg1, _arg2, R0, _immidiate, _address);
}

Instruction::Instruction(InstructionType _type, RegisterType _arg1, RegisterType _arg2,
    RegisterType _arg3, int _address) {
    init(_type, _arg1, _arg2, _arg3, 0, _address);
}

Instruction::Instruction(Instruction& inst) {
    init(inst.type, inst.arg1, inst.arg2, inst.arg3, inst.immidiate, inst.address);
    A = inst.A;
    B = inst.B;
    aluOut = inst.aluOut;
    loadMemData = inst.loadMemData;
    fetchedAtCycle = inst.fetchedAtCycle;
    srcCycle1 = inst.srcCycle1;
    srcCycle2 = inst.srcCycle2;
}

Instruction& Instruction::operator=(Instruction& rhs) {
    if (this == &rhs)
        return *this;
    init(rhs.type, rhs.arg1, rhs.arg2, rhs.arg3, rhs.immidiate, rhs.address);
    A = rhs.A;
    B = rhs.B;
    aluOut = rhs.aluOut;
    loadMemData = rhs.loadMemData;
    fetchedAtCycle = rhs.fetchedAtCycle;
    srcCycle1 = rhs.srcCycle1;
    srcCycle2 = rhs.srcCycle2;
    return *this;
}

void Instruction::clear() {
    init(NOP, R0, R0, R0, 0, 0);
}

// Returns whether dest instruction is data dependent on src
// This should be used for detecting hazard between in ID stage
// instruction and later stage instruction

//I am not using this, I am going to do it in my way!
bool Instruction::isDataDependent(Instruction& src, Instruction& dest) {
    return false;
}

Instruction::~Instruction() {}
