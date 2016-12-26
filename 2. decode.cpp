#include <iostream>
#include <string>
#include <cassert>
#include "decode.h"
using namespace std;

DecodeStage::DecodeStage(StageType _type, AbstractStage *_prevStage) : AbstractStage(_type, _prevStage) {
    assert(prevStage->getType() == IF);
}

int stall_needed = 0;
int count_data_stall = 0;
bool branch_stall = false;
bool branch_data_stall = false;
bool data_stall = false;

Instruction tempBranch;

void DecodeStage::process() {
    if (this->ins.getType() == HLT) {
        this->nextStage->setInstruction(this->ins);
        isHaltSeen = true;
        return;
    }

    if (this->ins.getType() == NOP) {
        if (branch_stall) {
            stall_needed--;
            if (stall_needed == 0) {
                branch_stall = false;
                this->prevStage->setUnstalled();
                this->nextStage->setInstruction(this->ins);
            } else {
                sendNOPToNextStage();
            }
        }
        return;
    }
    /*	[	1st and 2nd argument depends on Instruction
        [	0 = no data depencence!
        dataDependent = [	1 = data dependence in 1st argument
                        [	2 = data dependence in 1st argument
                        [	3 = 1 + 2
        */
    int dataDependent = checkDataDependency(this->ins);

    if (!this->ins.isBranch())
        this->ins.setBranchPc(0);

    if (dataDependent == 0) {
        // here is the actual processing
        readAndSetA();
        readAndSetB();
        readAndSetStoreData();
        if (this->prevStage->isStalled()) {
            if (data_stall) {
                this->prevStage->setUnstalled();
                data_stall = false;
                this->ins.setStalled(false);
            }
        }
        if (this->ins.isBranch()) {
            //set stall to previous stage
            this->getPrevStage()->setStalled();
            stall_needed = 2;
            branch_stall = true;

            //set the instruction to the next stage
            this->nextStage->setInstruction(this->ins);

            // now I do not  need to keep the instruction in this stage anymore.
            Instruction *n1 = new Instruction();
            this->ins = *n1;
            return;
        }

        if (this->ins.isAluReg() || this->ins.isAluImm() || this->ins.isLoad()) {
            lockRegister((RegisterType)(this->ins.getArg1()), this->ins.getFetchedAtCycle());
        }

        this->nextStage->setInstruction(this->ins);

        //now I do not  need to keep the instruction in this stage anymore.
        Instruction *n1 = new Instruction();
        this->ins = *n1;

    } else {
        data_stall = true;
        this->ins.setStalled(true);
        tempBranch = this->ins;
        this->prevStage->setStalled();

        sendNOPToNextStage();
    }
}

void DecodeStage::readAndSetA() {
    if (this->ins.isAluReg() || this->ins.isAluImm() || this->ins.isMemory() || this->ins.isBranch()) {
        this->ins.setA(getReg((RegisterType)(this->ins.getArg2())));
    }
}

void DecodeStage::readAndSetB() {
    if (this->ins.isAluReg()) {
        this->ins.setB(getReg((RegisterType)(this->ins.getArg3())));
    }
}

void DecodeStage::readAndSetStoreData() {
    if (this->ins.isStore()) {
        this->ins.setLoadMemData(getReg((RegisterType)(this->ins.getArg1())));
    }
}

void DecodeStage::sendNOPToNextStage() {
    Instruction *nop = new Instruction();
    this->nextStage->setInstruction(*nop);
}

DecodeStage::~DecodeStage() {}
