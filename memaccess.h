#ifndef MEMACCESS_H
#define MEMACCESS_H

#include "stage.h"

class MemaccessStage : protected AbstractStage {
protected:
    MemaccessStage(StageType _type, AbstractStage *_prevStage);

public:
    virtual void process();
    ~MemaccessStage();

    friend class AbstractStage;
};
#endif 
