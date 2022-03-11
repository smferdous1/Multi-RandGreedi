#include <cmath>

#include "Selection.h"
#include "Types.h"
#include "Utility.h"
using namespace std;
bool BMatchingSelection::init(const CSR& g){
    assert(g.nCol <= bMaxCol.size());
    initialized = true;
    return true;
}

bool BMatchingSelection::isBSaturated(Size row){
    return bCurrentCol[row] >= bMaxCol[row];
}

bool BMatchingSelection::calc_gain(const CSR& g, Val& m_gain, Size edgeIdx){
    
    if(!initialized){ 
        init(g);
        //cout << "initialized Selection obj" << endl;
    }
    if(edgeIdx>=g.nRow) return false;
    if(g.verPtr[edgeIdx+1] == g.verPtr[edgeIdx] + 1) {
        // cout << "Self loop" << endl;
        m_gain = 0;
        return true;
    }
    if(g.verPtr[edgeIdx+1] != g.verPtr[edgeIdx] + 2) {
        cout << "Input not a edge Adj matrix for edj " << edgeIdx << endl;
        return false;
    }
    
    
    Size u = g.verInd[g.verPtr[edgeIdx]].id;
    Size v = g.verInd[g.verPtr[edgeIdx]+1].id;
    Val w = g.verInd[g.verPtr[edgeIdx]].weight;
    
    if( !isBSaturated(u) && !isBSaturated(v)){
        
        m_gain = pow(totalGainBase+w, exponent) - totalGain;
    }
    else 
        m_gain = 0;
    
    // cout << "Marginal gain " << edgeIdx << ", (" << u << ", " << v << ", " << w << "): " << m_gain << "current total gain:" << totalGain << endl ;
    
    return true;
}

bool BMatchingSelection::update_selector(const CSR& g, Size edgeIdx){
    if(!initialized){ 
        init(g);
        //cout << "initialized Selection obj" << endl;
    }
    if(edgeIdx>=g.nRow) return false;
    
    Size u = g.verInd[g.verPtr[edgeIdx]].id;
    Size v = g.verInd[g.verPtr[edgeIdx]+1].id;
    Val w = g.verInd[g.verPtr[edgeIdx]].weight;
    
    if( !isBSaturated(u) && !isBSaturated(v) ){
        bCurrentCol[u]++;
        bCurrentCol[v]++;
        totalGainBase+= w;
        totalGain = pow(totalGainBase, exponent);
        selectedRows.push_back(edgeIdx);
        return true;
    }
    
    return false;
} //adds row to selecton and updates state


void BMatchingSelection::reset(Size k){
    totalGain = 0;
    totalGainBase = 0;
    nSelection = k;
    ResizeVector<Size>(&bCurrentCol, bMaxCol.size());
    ReserveVector<Size>(&selectedRows,nSelection);
    initialized = false;
}