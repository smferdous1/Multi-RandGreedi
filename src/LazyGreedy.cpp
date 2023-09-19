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
    Size count=0;
    
    for(Size i=0;i<g.nRow;i++){
        chk = s.calc_gain(g, gain, i);
        if(!chk) {
            cout << "Gain failed for " << i << endl;
            return false;
        }
        // if(i%250==0) cout << "Gain for " << i << " is " << gain << endl;
        count++;
        priQ[i] = make_pair(gain,i);
    }
    // cout << "PriQ initialized, Size:" << priQ.size() << endl;
    make_heap(priQ.begin(),priQ.end());
    // cout << "heap Made" << endl;
    
    for(Size i=0; i<k; i++){
        // cout << "Selecting object"<< i << endl;
    
        bool success= false;
        Size zeroCount;
        while(!success){
            
            if(priQ.size()==0){
                cout << "Priorty Queue Empty" << endl;
                i = k;
              break;  
            } 
            
            pop_heap(priQ.begin(),priQ.end()); 
            row = priQ.back().second;
            priQ.pop_back();
            //cout << "Recalculating Gain" << endl;
    
            chk = s.calc_gain(g, gain, row);
            count++;
            // cout << "gain for " << row << " is " << gain; 
            if(!chk){
                cout << "Gain failed for " << row << endl;
                return false;
            }
            
            // if(gain==0){
                // cout << "gain for row " << row << " was 0";
                // continue;
            // } 
            
            
            if( gain >= priQ.front().first){
                // cout << "Found object" << i << " PriQ Size: " << priQ.size() << endl;
                chk = s.update_selector(g,row);
                count++;
                if(!chk){
                    cout << "Update failed for " << row << endl;
                    return false;
                }
                success = true;
            }
            else
            {
                priQ.push_back(make_pair(gain,row));
                push_heap(priQ.begin(),priQ.end());
            }
        }
    }  
    
    // cout << "Selected rows: Actual:" << s.selectedRows.size() << " Expected: " << k << endl; 
    // cout << count << endl; 
    if(s.selectedRows.size()==k)
        return true;
    else return false;
}


