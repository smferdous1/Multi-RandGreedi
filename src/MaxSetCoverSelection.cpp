#ifndef SELECTION_H
#define SELECTION_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>

#include "CSR.h"
#include "Types.h"
using namespace std;

class MaxSetCoverSelection: public Selection{
    
    vector<bool> coveredCols;
    bool initialized;
    
    bool init(const CSR* g){
            ResizeVector(&coveredCols, g.nCol);
            initialized = true;
        }
        
    public:
        
        MaxSetCoverSelection():total_gain(0),initialized(false){}
        ~MaxSetCoverSelection(){}
    
};

bool MaxSetCoverSelection::calc_gain(const CSR* g, Val& m_gain, Size row){
    if(!initialized) init(g);
    Val marginal_gain = 0;
    Size col;
    for(i = g.verPtr[row]; i<= g.verPtr[row+1]; i++){
        col = verInd[i].id;
        if(!coveredCols[col])
            marginal_gain++;
    }
    m_gain= marginal_gain;
}

bool MaxSetCoverSelection::update_selector(const CSR* g, Size row){
        if(!initialized) init(g);
    Val marginal_gain = 0;
    Size col;
    for(i = g.verPtr[row]; i<= g.verPtr[row+1]; i++){
        col = verInd[i].id;
        if(!coveredCols[col]){
            marginal_gain++;
            coveredCols[col] = true;
        }
    }
    total_gain+=marginal_gain;
    selectedRows.push_back(row);
    } //adds row to selecton and updates state

#endif //SELECTION_H