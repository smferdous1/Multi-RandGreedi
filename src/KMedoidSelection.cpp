#include "Selection.h"
#include "Types.h"
#include "Utility.h"
using namespace std;
bool KMedoidSelection::init(const CSR& g){
    ResizeVector<Val>(&bestSimiliarityCol, g.nRow);
    std::fill(bestSimiliarityCol.begin(),bestSimiliarityCol.end(),negInfVal);
    initialized = true;
    return true;
}

bool KMedoidSelection::calc_gain(const CSR& g, Val& m_gain, Size row) {
    if(!initialized){ 
        init(g);
        // cout << "initialized Selection obj" << endl;
    }
    if(row>=g.nRow) return false;
    Val marginal_gain = 0;
    Val wgt;
    for(Size i = 0; i<g.nRow; i++){
        
        if(row==i) continue;
        
        g.getSimilarity(wgt,row,i);
        // cout << "checking col "<< i << " new = " << wgt << " old = " << bestSimiliarityCol[i]<< endl;
        if(bestSimiliarityCol[i]< wgt)
            marginal_gain+= wgt - bestSimiliarityCol[i];
    }
    m_gain= marginal_gain;
    // cout << "gain of row " << row << "is" << m_gain << endl;
    return true;
}

bool KMedoidSelection::update_selector(const CSR& g, Size row){
    if(!initialized){ 
        init(g);
        //cout << "initialized Selection obj" << endl;
    }
    if(row>=g.nRow) return false;
    
    Val marginal_gain = 0;
    Val wgt;
    for(Size i = 0; i<g.nRow; i++){
        
        if(row==i) continue;
        
        g.getSimilarity(wgt,row,i);
        // cout << "checking col "<< i << " new = " << wgt << " old = " << bestSimiliarityCol[i]<< endl;
        if(bestSimiliarityCol[i]< wgt){
            marginal_gain+= wgt - bestSimiliarityCol[i];
            bestSimiliarityCol[i] = wgt;
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