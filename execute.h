#ifndef EXECUTE_H
#define EXECUTE_H

#include "stage.h"

class ExecuteStage : protected AbstractStage {
protected:
    ExecuteStage(StageType _type, AbstractStage *_prevStage);
public:
    virtual void process();
    void ExecuteStage::doOperation(Instruction &inst, int a, int b);
    ~ExecuteStage();

    friend class AbstractStage;
};
#endif 
