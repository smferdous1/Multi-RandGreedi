#pragma once

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
    
    virtual bool calc_gain(const CSR& g, Val& m_gain, Size row) = 0; // calculates the marginal gain of adding row into the selection
    
    virtual bool update_selector(const CSR& g, Size row)= 0; //adds row to selecton and updates state.
    
    virtual void reset(Size k) = 0; 
    
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
        void reset(Size k);
        
        
        MaxSetCoverSelection(Size k):initialized(false),Selection(k){}
        ~MaxSetCoverSelection(){}
    
};

class KMedoidSelection: public Selection{
    
    vector<Val> bestSimiliarityCol;
    bool initialized;
    
    bool init(const CSR& g);
        
    public:
        bool calc_gain(const CSR& g, Val& m_gain, Size row);
        bool update_selector(const CSR& g, Size row);
        void reset(Size k);
        
        
        KMedoidSelection(Size k):initialized(false),Selection(k){}
        ~KMedoidSelection(){}
    
};

class BMatchingSelection: public Selection{
    // working only upper triangular matrix;
    vector<Size> bMaxCol, bCurrentCol;
    Val exponent;
    bool initialized;
    Val totalGainBase;
    
    
    bool init(const CSR& g);
    bool isBSaturated(Size row);
    public:
        bool calc_gain(const CSR& g, Val& m_gain, Size edgIdx);
        bool update_selector(const CSR& g, Size edgIdx);
        void reset(Size k);
        
        
        BMatchingSelection(vector<Size>& bMaxColumn, Size k):initialized(false),totalGainBase(0),Selection(k){
            ResizeVector<Size>(&bMaxCol, bMaxColumn.size());
            ResizeVector<Size>(&bCurrentCol, bMaxColumn.size());
            
            for(Size i = 0; i <bMaxColumn.size(); i ++){
                bMaxCol[i]=bMaxColumn[i];
            }
                
        }
        ~BMatchingSelection(){}
    
};
