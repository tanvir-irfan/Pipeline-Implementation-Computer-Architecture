#include <cassert>
#include <iostream>
#include "fetch.h"



FetchStage::FetchStage(StageType _type, AbstractStage *_prevStage) : AbstractStage(_type, _prevStage) {
    assert(prevStage == NULL);
}

void FetchStage::process() {
    Instruction inst;
    Instruction *nop = new Instruction();
    if (this->nextStage->isHaltSeen) {
        incStatistics(STALLCYCLE);
        return;
    }
    if (this->isStalled()) {
        if (this->nextStage->getInstruction().isStalled()) {
            // do nothing!
        } else {
            this->nextStage->setInstruction(*nop);
        }
        incStatistics(STALLCYCLE);
        return;

    } else {
        inst = getNextIns();

        incStatistics(FETCHEDINS);
        setPc(getPc() + 4);

        inst.setFetchedAtCycle(getCycle());

        this->nextStage->setInstruction(inst);
    }
}

FetchStage::~FetchStage() {}
