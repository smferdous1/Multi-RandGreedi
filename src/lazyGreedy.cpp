#include "Optimizer.h"
#include "Utility.h"
using namespace std;


bool LazyGreedy::select(const CSR& g, Selection& s,Size k){
    
    if(g.nRow<k) return false;
    
    vector<pair<Val,Size>> priQ;
    ResizeVector(&priQ, g.nRow);
    Val gain;
    Size row;
    
    for(Size i=0;i<g.nRow;i++){
        s.calc_gain(g, gain, i);
        priQ[i] = make_pair(gain,i);
    }
    make_heap(priQ.begin(),priQ.end());
    
    
    for(Size i =1; i<k; i++){
        bool success= false;
        while(!success){
            pop_heap(priQ.begin(),priQ.end()); 
            row = priQ.back().second;
            priQ.pop_back();
            s.calc_gain(g, gain, i);
            
            if( gain > priQ.front().first){
                s.update_selector(g,row);
                success = true;
            }
            else
            {
                priQ.push_back(make_pair(gain,row));
                push_heap(priQ.begin(),priQ.end());
            }
        }
    }  
    
    return true;
}


