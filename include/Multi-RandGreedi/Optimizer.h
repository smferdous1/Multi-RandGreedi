#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <iostream>

#include "CSR.h"
#include "Selector.h"
using namespace std;

class Optimizer
{
    public:
    
    bool select(const CSR* g, Selector* s,int k) = 0; // virtual optimizer
    
};

#endif //OPTIMIZER_H
#include "CSR.h"
#include "Selector.h"
using namespace std;

class Optimizer
{
    public:
    
    bool select(const CSR* g, Selector* s,int k) = 0; // virtual optimizer
    
};

#endif //OPTIMIZER_H