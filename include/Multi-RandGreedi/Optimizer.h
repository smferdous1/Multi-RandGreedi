#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "CSR.h"
#include "Selection.h"
#include "Types.h"

class Optimizer
{
    public:
    
    virtual bool select(const CSR& g, Selection& s,Size k) = 0; // virtual optimizer
    
};

class LazyGreedy: public Optimizer{
    public:
    bool select(const CSR& g, Selection& s,Size k);
};


class GreeDiL: public Optimizer{
    public:
        Optimizer o;
        Size machCapacity;
    bool select(const CSR& g, Selection& s,Size k);
    
    GreeDiL(Optimizer opti, Size mSize):machCapacity(mSize){ 
        o = opti; 
    }
    ~GreeDiL(){}
    
};

#endif //OPTIMIZER_H
