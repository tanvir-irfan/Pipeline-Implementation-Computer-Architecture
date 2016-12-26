#include <cassert>
#include "writeback.h"

WritebackStage::WritebackStage(StageType _type, AbstractStage *_prevStage) : AbstractStage(_type, _prevStage) {
    assert(prevStage->getType() == MEM);
}

void WritebackStage::process() {
    if (this->ins.getType() == HLT) {
        stopSimulation();
        incStatistics(FINISHEDINS);
        return;
    }

    if (this->ins.getType() == NOP) {
        return;
    }
    incStatistics(FINISHEDINS);
    if (this->ins.isAluReg() || this->ins.isAluImm()) {
        setReg((RegisterType)(this->ins.getArg1()), this->ins.getAluOut());
    } else if (this->ins.isLoad()) {
        setReg((RegisterType)(this->ins.getArg1()), this->ins.getLoadMemData());
    }

    if (this->ins.isAluReg() || this->ins.isAluImm() || this->ins.isLoad()) {
        this->getPrevStage()->getPrevStage()->getPrevStage()->unlockRegister((RegisterType)(this->ins.getArg1()), this->ins.getFetchedAtCycle());
    }
}

WritebackStage::~WritebackStage() {}
