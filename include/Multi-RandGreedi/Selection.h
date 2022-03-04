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
#include "Utility.h"
using namespace std;

class Selection
{
    public:
        Val totalGain;
        vector<Size> selectedRows;
        Size nSelection;
    
    virtual bool calc_gain(const CSR& g, Val& m_gain, Size row)= 0; // calculates the marginal gain of adding row into the selection
    
    virtual bool update_selector(const CSR& g, Size row)= 0; //adds row to selecton and updates state.
    Selection(Size k):totalGain(0),nSelection(k){
        ReserveVector<Size>(&selectedRows,nSelection);
    }
    ~Selection(){}
  
};


class MaxSetCoverSelection: public Selection{
    
    vector<bool> coveredCols;
    bool initialized;
    
    bool init(const CSR& g);
        
    public:
        bool calc_gain(const CSR& g, Val& m_gain, Size row);
        bool update_selector(const CSR& g, Size row);
        
        MaxSetCoverSelection(Size k):initialized(false),Selection(k){}
        ~MaxSetCoverSelection(){}
    
};


#endif //SELECTION_H