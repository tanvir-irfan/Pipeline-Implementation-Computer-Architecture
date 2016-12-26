#ifndef STAGE_H
#define STAGE_H

#include <iostream>
#include <cstddef>
#include "instruction.h"

using namespace std;

typedef enum _StageType { IF = 0, ID, EX, MEM, WR, TOTAL_STAGE_TYPE } StageType;
typedef enum _StatisticsType { STALLCYCLE = 0, FETCHEDINS, FINISHEDINS, TOTAL_STATISTICS_TYPE } StatisticsType;


class AbstractStage {
private:
    int reg_locked_for_write[TOTAL_REGISTER_TYPE];
protected:
    typedef int(*getPcPtr)();
    static getPcPtr getPc;
    typedef void(*setPcPtr)(int);
    static setPcPtr setPc;
    typedef Instruction& (*getNextInsPtr)();
    static getNextInsPtr getNextIns;
    typedef int(*getRegPtr)(RegisterType);
    static getRegPtr getReg;
    typedef void(*setRegPtr)(RegisterType, int);
    static setRegPtr setReg;
    typedef int(*getDataPtr)(int);
    static getDataPtr getData;
    typedef void(*setDataPtr)(int, int);
    static setDataPtr setData;
    typedef void(*setStopSimulationPtr)();
    static setStopSimulationPtr stopSimulation;
    typedef int(*getCyclePtr)();
    static getCyclePtr getCycle;
    typedef void(*incStatisticsPtr)(StatisticsType);
    static incStatisticsPtr incStatistics;

    Instruction ins;
    bool stalled;
    StageType type;
    AbstractStage *prevStage, *nextStage;
    AbstractStage(StageType _type, AbstractStage *_prevStage);

public:
    int stall_needed_for_branch = 2;
    bool stall_for_branch = false;
    bool stall_for_data = false;
    bool isHaltSeen = false;
    virtual void process() = 0;
    static AbstractStage* createStage(StageType _type, AbstractStage *_prevStage);
    inline StageType getType() { return type; }
    inline bool isStalled() { return stalled; }
    void setStalled();
    void setUnstalled();
    void updateDependences(int _srcCycle);
    void setInstruction(Instruction& _ins);
    void printDebugLog();
    bool isRegLocked(int index);
    void lockRegister(int index, int cc);
    int AbstractStage::registerLockedBy(int index);
    void unlockRegister(int index, int cc);
    int checkDataDependency(Instruction& _ins);
    Instruction& getInstruction();
    inline AbstractStage* getPrevStage() { return prevStage; }
    static inline void setPtrGetPc(int(*_getPc)()) { getPc = _getPc; }
    static inline void setPtrSetPc(void(*_setPc)(int)) { setPc = _setPc; }
    static inline void setPtrGetNextIns(Instruction& (_getNextIns)()) { getNextIns = _getNextIns; }
    static inline void setPtrGetReg(int(*_getReg)(RegisterType)) { getReg = _getReg; }
    static inline void setPtrSetReg(void(*_setReg)(RegisterType, int)) { setReg = _setReg; }
    static inline void setPtrGetData(int(*_getData)(int)) { getData = _getData; }
    static inline void setPtrSetData(void(*_setData)(int, int)) { setData = _setData; }
    static inline void setPtrStopSimulation(void(*_stopSimulation)()) { stopSimulation = _stopSimulation; }
    static inline void setPtrGetCycle(int(*_getCycle)()) { getCycle = _getCycle; }
    static inline void setPtrIncStatistics(void(*_incStatistics)(StatisticsType)) { incStatistics = _incStatistics; }
    virtual ~AbstractStage();
};

#endif
