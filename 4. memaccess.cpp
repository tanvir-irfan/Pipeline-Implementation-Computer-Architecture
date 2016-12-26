#include <cassert>
#include "memaccess.h"

MemaccessStage::MemaccessStage(StageType _type, AbstractStage *_prevStage) : AbstractStage(_type, _prevStage) {
    assert(prevStage->getType() == EX);
}

void MemaccessStage::process() {

    if (this->ins.getType() == HLT) {
        this->nextStage->setInstruction(this->ins);
        return;
    }

    int addr = 0;
    int data = 0;

    switch (this->ins.getType()) {
    case LD:
        addr = this->ins.getAluOut();
        data = getData(addr);
        this->ins.setLoadMemData(data);
        break;
    case ST:
        addr = this->ins.getAluOut();
        data = this->ins.getLoadMemData();
        setData(addr, data);
        break;
    }

    this->nextStage->setInstruction(this->ins);
}

MemaccessStage::~MemaccessStage() {}
