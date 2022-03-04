#include "Selection.h"
#include "Types.h"
#include "Utility.h"
bool MaxSetCoverSelection::init(const CSR& g){
    ResizeVector(&coveredCols, g.nCol);
    initialized = true;
}

bool MaxSetCoverSelection::calc_gain(const CSR& g, Val& m_gain, Size row){
    if(!initialized) init(g);
    Val marginal_gain = 0;
    Size col;
    for(Size i = g.verPtr[row]; i<= g.verPtr[row+1]; i++){
        col = g.verInd[i].id;
        if(!coveredCols[col])
            marginal_gain++;
    }
    m_gain= marginal_gain;
}

bool MaxSetCoverSelection::update_selector(const CSR& g, Size row){
        if(!initialized) init(g);
    Val marginal_gain = 0;
    Size col;
    for(Size i = g.verPtr[row]; i<= g.verPtr[row+1]; i++){
        col = g.verInd[i].id;
        if(!coveredCols[col]){
            marginal_gain++;
            coveredCols[col] = true;
        }
    }
    totalGain+=marginal_gain;
    selectedRows.push_back(row);
    } //adds row to selecton and updates state
