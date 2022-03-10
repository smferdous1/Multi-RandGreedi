#include "Selection.h"
#include "Types.h"
#include "Utility.h"
using namespace std;
bool MaxSetCoverSelection::init(const CSR& g){
    Size col = g.nCol;
    ResizeVector<bool>(&coveredCols, col);
    initialized = true;
    return true;
}

bool MaxSetCoverSelection::calc_gain(const CSR& g, Val& m_gain, Size row){
    if(!initialized){ 
        init(g);
        //cout << "initialized Selection obj" << endl;
    }
    if(row>=g.nRow) return false;
    Val marginal_gain = 0;
    Size col;
    for(Size i = g.verPtr[row]; i< g.verPtr[row+1]; i++){
        col = g.verInd[i].id;
        //cout << "checking col "<< col << endl;
        if(!coveredCols[col])
            marginal_gain++;
    }
    m_gain= marginal_gain;
    //cout << "gain of row " << row << "is" << m_gain << endl;
    return true;
}

bool MaxSetCoverSelection::update_selector(const CSR& g, Size row){
    if(!initialized){ 
        init(g);
        //cout << "initialized Selection obj" << endl;
    }
    if(row>=g.nRow) return false;
    
    Val marginal_gain = 0;
    Size col;
    for(Size i = g.verPtr[row]; i< g.verPtr[row+1]; i++){
        col = g.verInd[i].id;
        if(!coveredCols[col]){
            //cout << "masking col "<< col << endl;
            marginal_gain++;
            coveredCols[col] = true;
        }
    }
    //cout << "Finished masking "<< endl;
    totalGain+=marginal_gain;
    
    selectedRows.push_back(row);
    //cout << "Pushed into selected rows" << endl;
    
    return true;
} //adds row to selecton and updates state


void MaxSetCoverSelection::reset(Size k){
    totalGain = 0;
    nSelection = k;
    ReserveVector<Size>(&selectedRows,nSelection);
    initialized = false;
}