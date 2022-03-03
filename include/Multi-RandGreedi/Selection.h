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

class Selection
{
    public:
        Val total_gain;
        vector<Size> selectedRows;
    
    bool calc_gain(const CSR* g, Val& m_gain, Size row)= 0; // calculates the marginal gain of adding row into the selection
    
    bool update_selector(const CSR* g, Size row)= 0; //adds row to selecton and updates state.
    
  
};

#endif //SELECTION_H