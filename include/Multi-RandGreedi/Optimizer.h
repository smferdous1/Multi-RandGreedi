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
        Optimizer* localOptimizer;
        Size branchingFactor;
    bool select(const CSR& g, Selection& s,Size k);
    
    // If p_branch is tru then the parameter is the branching factor else it is the size of the machine.
    
    GreeDiL(Optimizer* opti, Size brnchFctr):branchingFactor(brnchFctr){ 
        localOptimizer = opti; 
    }
    ~GreeDiL(){ }
    
};

#endif //OPTIMIZER_H
