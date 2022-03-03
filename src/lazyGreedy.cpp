#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <queue>

#include "CSR.h"
#include "Utility.h"
#include "Types.h"
#include "Optimizer.h"
#include "Selection.h"
using namespace std;


class LazyGreedy: public Optimizer{
public:
    bool select(const CSR& g, Selection& s,Size k){
        
        if(nRow<k) return false;
        
        s = new Selection();
        vector<tuple<Val,Size>> priQ;
        ResizeVector(&priQ, g.nRow);
        Val gain;
        Size row;
        
        for(Size i=0;i<nRow;i++){
            s.calc_gain(g, gain, i);
            priQ[i] = make_tuple(gain,i);
        }
        make_heap(priQ.begin(),priQ.end());
        
        
        for(Size i =1; i<k; i++){
            bool success= false;
            while(!success){
                pop_heap(priQ.begin(),priQ.end()); 
                row = get<1>priQ.pop_back();
                s.calc_gain(g, gain, i);
                
                if( gain > get<0>priQ.front()){
                    s.update_selector(g,row);
                    success = true;
                }
                else{
                    priQ.push_back(make_tuple(gain,row);
                    push_heap(priQ.begin(),priQ.end());
                }
            }
        }  
        
        return true;
    }
}

