#pragma once

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
    
    
    GreeDiL(Optimizer* opti, Size brnchFctr):branchingFactor(brnchFctr){ 
        localOptimizer = opti; 
    }
    ~GreeDiL(){ }
    
};


class GreeDiL2: public Optimizer{
    public:
        Optimizer* localOptimizer;
        Size branchingFactor;
        char* filename;
    bool select(const CSR& g, Selection& s,Size k);
    
   
    GreeDiL2(Optimizer* opti, Size brnchFctr, char* file):branchingFactor(brnchFctr){ 
        localOptimizer = opti; 
        filename = file;
    }
    ~GreeDiL2(){}
    
};

#pragma once
