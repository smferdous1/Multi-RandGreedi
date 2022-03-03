#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <queue>

#include "CSR.h"
#include "Utility.h"
#include "Optimizer.h"
#include "Selection.h"



class LazyGreedy: public Optimizer{
public:
    bool select(const CSR* g, Selection* s,int k){
        
        std::priority_queue<KeyValue<int,int>, std::vector<KeyValue<int,int>>, std::function<bool(Foo, Foo)>> pq(Compare);
    
        for(int i=1;i<=nRow;i++)
        {
            
            
        return false;
    }
}

