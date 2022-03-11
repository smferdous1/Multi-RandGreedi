#include "Optimizer.h"
#include "Utility.h"
using namespace std;


bool LazyGreedy::select(const CSR& g, Selection& s,Size k){
    
    if(g.nRow<k) return false;
    s.reset(k);
    vector<pair<Val,Size>> priQ;
    ResizeVector<pair<Val,Size>>(&priQ, g.nRow);
    Val gain;
    Size row;
    bool chk;
    
    for(Size i=0;i<g.nRow;i++){
        chk = s.calc_gain(g, gain, i);
        if(!chk) return false;
        
        priQ[i] = make_pair(gain,i);
    }
    //cout << "PriQ initialized" << endl;
    make_heap(priQ.begin(),priQ.end());
    //cout << "heap Made" << endl;
    
    
    
    for(Size i=0; i<k; i++){
        //cout << "Selecting object"<< i << endl;
    
        bool success= false;
        Size zeroCount;
        while(!success){
            
            if(priQ.size()==0) break;
            
            pop_heap(priQ.begin(),priQ.end()); 
            row = priQ.back().second;
            priQ.pop_back();
            //cout << "Recalculating Gain" << endl;
    
            chk = s.calc_gain(g, gain, row);
            if(!chk) return false;
            
            if(gain==0) continue;
            
            if( gain >= priQ.front().first){
                //cout << "Found object" << i << endl;
                chk = s.update_selector(g,row);
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


