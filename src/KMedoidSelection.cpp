#include "Selection.h"
#include "Types.h"
#include "Utility.h"
using namespace std;
bool KMedoidSelection::init(const CSR& g){
    ResizeVector<Val>(&bestSimiliarityCol, g.nCol);
    initialized = true;
    return true;
}

bool KMedoidSelection::calc_gain(const CSR& g, Val& m_gain, Size row) {
    if(!initialized){ 
        init(g);
        //cout << "initialized Selection obj" << endl;
    }
    if(row>=g.nRow) return false;
    Val marginal_gain = 0;
    Size col;
    Val wgt;
    for(Size i = g.verPtr[row]; i< g.verPtr[row+1]; i++){
        col = g.verInd[i].id;
        wgt = g.verInd[i].weight;
        //cout << "checking col "<< col << endl;
        if(bestSimiliarityCol[col]< wgt)
            marginal_gain+= wgt - bestSimiliarityCol[col];
    }
    m_gain= marginal_gain;
    //cout << "gain of row " << row << "is" << m_gain << endl;
    return true;
}

bool KMedoidSelection::update_selector(const CSR& g, Size row){
    if(!initialized){ 
        init(g);
        //cout << "initialized Selection obj" << endl;
    }
    if(row>=g.nRow) return false;
    
    Val marginal_gain = 0;
    Size col;
    Val wgt;
    for(Size i = g.verPtr[row]; i< g.verPtr[row+1]; i++){
        col = g.verInd[i].id;
        wgt = g.verInd[i].weight;
        if(bestSimiliarityCol[col]< wgt){
            //cout << "updating col "<< col << endl;
            marginal_gain+= wgt - bestSimiliarityCol[col];
            bestSimiliarityCol[col] = wgt;
        }
    }
    //cout << "Finished masking "<< endl;
    totalGain+=marginal_gain;
    
    selectedRows.push_back(row);
    //cout << "Pushed into selected rows" << endl;
    
    return true;
} //adds row to selecton and updates state


void KMedoidSelection::reset(Size k){
    totalGain = 0;
    nSelection = k;
    ReserveVector<Size>(&selectedRows,nSelection);
    initialized = false;
}