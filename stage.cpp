#include <cassert>
#include "stage.h"
#include "fetch.h"
#include "decode.h"
#include "execute.h"
#include "memaccess.h"
#include "writeback.h"

AbstractStage::getPcPtr AbstractStage::getPc = NULL;
AbstractStage::setPcPtr AbstractStage::setPc = NULL;
AbstractStage::getNextInsPtr AbstractStage::getNextIns = NULL;
AbstractStage::getRegPtr AbstractStage::getReg = NULL;
AbstractStage::setRegPtr AbstractStage::setReg = NULL;
AbstractStage::getDataPtr AbstractStage::getData = NULL;
AbstractStage::setDataPtr AbstractStage::setData = NULL;
AbstractStage::setStopSimulationPtr AbstractStage::stopSimulation = NULL;
AbstractStage::getCyclePtr AbstractStage::getCycle = NULL;
AbstractStage::incStatisticsPtr AbstractStage::incStatistics = NULL;

//tanvirirfan.utsa _start
bool debug_flag = false;
//tanvirirfan.utsa _start

AbstractStage::AbstractStage(StageType _type, AbstractStage *_prevStage) : stalled(false), type(_type),
prevStage(_prevStage) {
    for (int i = 0; i < TOTAL_REGISTER_TYPE; i++)
        reg_locked_for_write[i] = 0;
    if (prevStage != NULL)
        prevStage->nextStage = this;
    nextStage = NULL;
}

AbstractStage* AbstractStage::createStage(StageType _type, AbstractStage *_prevStage) {
    // Create a new stage of type _type and set the previous stage to _prevStage
    // Return the newly created stage
    AbstractStage* temp;
    switch (_type) {
    case IF:
        temp = new FetchStage(_type, _prevStage);
        break;
    case ID:
        temp = new DecodeStage(_type, _prevStage);
        break;
    case EX:
        temp = new ExecuteStage(_type, _prevStage);
        break;
    case MEM:
        temp = new MemaccessStage(_type, _prevStage);
        break;
    case WR:
        temp = new WritebackStage(_type, _prevStage);
        break;
    }
    return temp;
}

void AbstractStage::setStalled() {
    stalled = true;
    /*
    if (prevStage)
    prevStage->setStalled();
    */
}

void AbstractStage::setUnstalled() {
    stalled = false;
    /*
    if (prevStage)
    prevStage->setUnstalled();
    */
}

void AbstractStage::updateDependences(int _srcCycle) {
    // This function is used to notify (and possibly unstall) the consumer instruction that was stalled
    // due to the current instruction (i.e., producer instruction) of this stage
}

void AbstractStage::setInstruction(Instruction& _ins) {
    ins = _ins;
}

Instruction& AbstractStage::getInstruction() {
    return ins;
}

bool AbstractStage::isRegLocked(int index) {
    return reg_locked_for_write[index] != 0;
}

void AbstractStage::lockRegister(int index, int cc) {
    reg_locked_for_write[index] = cc;
}

int AbstractStage::registerLockedBy(int index) {
    return reg_locked_for_write[index];
}

void AbstractStage::unlockRegister(int index, int cc) {
    if (reg_locked_for_write[index] == cc) {
        reg_locked_for_write[index] = 0;
    }
}

int AbstractStage::checkDataDependency(Instruction& _ins) {
    int dependent = 0;


    if (_ins.isAluReg()) {
        if (this->isRegLocked((RegisterType)(_ins.getArg2()))) {
            dependent += 1;
            _ins.setSrcCycle1(reg_locked_for_write[(RegisterType)(_ins.getArg2())]);
        }
        if (this->isRegLocked((RegisterType)(_ins.getArg3()))) {
            dependent += 2;
            _ins.setSrcCycle2(reg_locked_for_write[(RegisterType)(_ins.getArg3())]);
        }
    } else if (_ins.isAluImm()) {
        if (this->isRegLocked((RegisterType)(_ins.getArg2()))) {
            dependent += 1;
            _ins.setSrcCycle1(reg_locked_for_write[(RegisterType)(_ins.getArg2())]);
        }
    } else if (_ins.isBranch()) {
        if (this->isRegLocked((RegisterType)(_ins.getArg1()))) {
            dependent += 1;
            _ins.setSrcCycle1(reg_locked_for_write[(RegisterType)(_ins.getArg1())]);
        }
    } else if (_ins.isLoad()) {
        if (this->isRegLocked((RegisterType)(_ins.getArg2()))) {
            dependent += 1;
            _ins.setSrcCycle1(reg_locked_for_write[(RegisterType)(_ins.getArg2())]);
        }
    } else if (_ins.isStore()) {
        if (this->isRegLocked((RegisterType)(_ins.getArg2()))) {
            dependent += 1;
            _ins.setSrcCycle1(reg_locked_for_write[(RegisterType)(_ins.getArg2())]);
        }
        if (this->isRegLocked((RegisterType)(_ins.getArg1()))) {
            dependent += 2;
            _ins.setSrcCycle2(reg_locked_for_write[(RegisterType)(_ins.getArg1())]);
        }
    }
    return dependent;
}

AbstractStage::~AbstractStage() {}
