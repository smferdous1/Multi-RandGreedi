#ifndef SELECTION_H
#define SELECTION_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>

#include "CSR.h"
using namespace std;

class Selection
{
    public:
        double total_gain;
        vector<int> selectedRows;
    
    bool calc_gain(const CSR* g, double* m_gain, int row); // calculates the marginal gain of adding row into the selection
    
    bool update_selector(const CSR* g, int row); //adds row to selecton and updates state.

    
};

#endif //SELECTION_H