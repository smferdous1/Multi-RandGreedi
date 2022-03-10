#include <cmath>

#include "Selection.h"
#include "Types.h"
#include "Utility.h"
using namespace std;
bool BMatchingSelection::init(const CSR& g){
    assert(g.nCol <= bMaxCol.size());
    assert(g.nCol == g.nRow);
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
    if(edgeIdx>=g.nNz) return false;
    
    Edge e = g.verInd[edgeIdx];
    Size row;
    g.getRow(row,edgeIdx);
    
    if( !isBSaturated(row) && !isBSaturated(e.id) && (row >= e.id)){
        
        m_gain = pow(totalGainBase+e.weight, exponent) - totalGain;
        return true;
    }
    m_gain = 0;
    return true;
}

bool BMatchingSelection::update_selector(const CSR& g, Size edgeIdx){
    if(!initialized){ 
        init(g);
        //cout << "initialized Selection obj" << endl;
    }
    if(edgeIdx>=g.nNz) return false;
    
    Edge e = g.verInd[edgeIdx];
    Size row;
    g.getRow(row,edgeIdx);
    
    if( !isBSaturated(row) && !isBSaturated(e.id) && (row >= e.id) ){
        bCurrentCol[row]++;
        bCurrentCol[e.id]++;
        totalGainBase+= e.weight;
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