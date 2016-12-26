#include <cassert>
#include "execute.h"

ExecuteStage::ExecuteStage(StageType _type, AbstractStage *_prevStage) : AbstractStage(_type, _prevStage) {
    assert(prevStage->getType() == ID);
}

void ExecuteStage::process() {

    if (this->ins.getType() == HLT) {
        this->nextStage->setInstruction(this->ins);
        return;
    }

    if (this->ins.getType() == NOP) {
        Instruction *nop = new Instruction();
        this->nextStage->setInstruction(*nop);
        return;
    }

    int a = 0, b = 0;

    if (this->ins.isAluReg()) {
        a = ins.getA();
        b = ins.getB();

        doOperation(this->ins, a, b);
    } else if (this->ins.isAluImm() || this->ins.isMemory()) {
        a = ins.getA();
        b = ins.getImmidiate();

        doOperation(this->ins, a, b);
    } else if (this->ins.isBranch()) {
        a = ins.getAddress() + 4;
        b = this->ins.getImmidiate() * 4;

        doOperation(this->ins, a, b);
        //checking the condition here
        bool branchTaken = false;
        // I should do it in ID stage, but I am doing it here anyway!
        int conditionReg = getReg((RegisterType)(this->ins.getArg1()));

        switch (this->ins.getType()) {
        case BNEQZ:
            branchTaken = conditionReg != 0;
            break;
        case BEQZ:
            branchTaken = conditionReg == 0;
            break;
        }

        if (branchTaken) {
            setPc(this->ins.getAluOut());
        } else {
            setPc(a);
        }
    }

    //when processing done in this stage, pass the instruction to next stage
    if (this->ins.getType() != NOP)
        this->nextStage->setInstruction(this->ins);
}

void ExecuteStage::doOperation(Instruction &inst, int a, int b) {
    int result = 0;
    switch (inst.getType()) {
    case NOP:     // NOP          : Do nothing
        break;
    case ADD:     // ADD R1 R2 R3 : R1 = R2 + R3
    case ADDI:    // ADDI R1 R2 100: R1 = R2 + 100 
    case LD:      // LD  R1 R2 32 : R1 = Mem[R2+32]
    case ST:      // ST  R1 R2 32 : Mem[R2+32] = R1 
    case BEQZ:    // BEQZ R1 100  : if R1==0 goto NPC + 4 *100
    case BNEQZ:   // BNEQZ R1 100 : if R1!= 0 goto NPC + 4*100
        result = a + b;
        break;

    case SUB:     // SUB R1 R2 R3 : R1 = R2 - R3
    case SUBI:    // SUBI R1 R2 100: R1 = R2 - 100 
        result = a - b;
        break;

    case MUL:     // MUL R1 R2 R3 : R1 = R2 * R3
        result = a * b;
        break;
    case DIV:     // DIV R1 R2 R3 : R1 = R2 / R3
        result = a / b;
        break;
    case XOR:     // XOR R1 R2 R3 : R1 = R2 ^ R3
        result = a ^ b;
        break;
    case AND:     // AND R1 R2 R3 : R1 = R2 & R3
        result = a & b;
        break;
    case OR:      // OR  R1 R2 R3 : R1 = R2 | R3
        result = a | b;
        break;

    case HLT:     // HLT		
        break;
    default:
        break;
    }

    inst.setAluOut(result);
}
ExecuteStage::~ExecuteStage() {}
