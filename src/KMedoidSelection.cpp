#include "Selection.h"
#include "Types.h"
#include "Utility.h"
using namespace std;

/*
K-medoid: K data points that are closes to all other data points.
The function we maximize is f(S)= L({0})- L(S \cup {0})
L(P) is the minimum distance from all points to the points in P.
We set the zero vector as the auxillary element included in the set.


bestSimilarityCol[i] stores the distance from the closest point in the selection to i.
Marginal gain M(u)
                = f(S \cup {u})- f(S) 
                = L(S \cup {u})- L(S \cup {0} \cup {0})
*/


bool KMedoidSelection::init(const CSR& g){
    ResizeVector<Val>(&bestSimiliarityCol, g.nRow);
    Val sim=0;
    for(int i =0; i<g.nRow; i++){
        g.getSimilarity(sim,i,-1);
        bestSimiliarityCol[i]=sim;
    }
    
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
        
        if(row==i){ 
            marginal_gain+= bestSimiliarityCol[i];
        } 
        else{
            g.getSimilarity(wgt,row,i);
            
            if(wgt==0){
                
            }
            // cout << "checking col "<< i << " new = " << wgt << " old = " << bestSimiliarityCol[i]<< endl;
            if(bestSimiliarityCol[i]> wgt)
                marginal_gain+= bestSimiliarityCol[i] - wgt;
        }
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
        
        if(row==i){ 
            marginal_gain+= bestSimiliarityCol[i];
            bestSimiliarityCol[i]=0;
        } 
        else{
            g.getSimilarity(wgt,row,i);
            // cout << "checking col "<< i << " new = " << wgt << " old = " << bestSimiliarityCol[i]<< endl;
            if(bestSimiliarityCol[i]> wgt){
                marginal_gain+= bestSimiliarityCol[i] - wgt;
                bestSimiliarityCol[i] = wgt;
            }
        }
    }
    //cout << "Finished masking "<< endl;
    totalGain+=marginal_gain;
    
    selectedRows.push_back(row);
    // cout << "Pushed " << row << " into selected rows" << endl;
    
    return true;
} //adds row to selecton and updates state


void KMedoidSelection::reset(Size k){
    totalGain = 0;
    nSelection = k;
    ReserveVector<Size>(&selectedRows,nSelection);
    initialized = false;
}