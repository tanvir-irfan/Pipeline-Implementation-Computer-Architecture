#ifndef WRITEBACK_H
#define WRITEBACK_H

#include "stage.h"

class WritebackStage : protected AbstractStage {
protected:
    WritebackStage(StageType _type, AbstractStage *_prevStage);

public:
    virtual void process();
    ~WritebackStage();

    friend class AbstractStage;
};
#endif 
