#ifndef FETCH_H
#define FETCH_H

#include "stage.h"

class FetchStage : protected AbstractStage {
protected:
    FetchStage(StageType _type, AbstractStage *_prevStage);
public:
    virtual void process();
    ~FetchStage();

    friend class AbstractStage;
};
#endif 
