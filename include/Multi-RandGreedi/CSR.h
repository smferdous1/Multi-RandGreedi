#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>

#include "Types.h"

using namespace std;

struct EdgeE {
    Size head;
    Size id;         // Edge tail
    float weight;  // Edge weight
};
struct Edge {
    Size id;         // Edge tail
    Val weight;  // Edge weight
};

class CSR {
    public:
    Size nRow;       // number of rows
    Size nCol;       // number of columns
    Size nNz;      // number of nonzeros
    Size maxDeg;
    vector<Size> verPtr;    // vertex pointer array of size nRow+1
    vector<Edge> verInd;   // Edge array
    
    bool readMtx(char * filename); // reading as a general graph
    bool getRow(Size& row, Size edgId) const;
    bool getSubmatrix( CSR& subMtx, vector<Size>& rowIdxs) const ;
    
    
    CSR():nRow(0),nCol(0),nNz(0){}
    ~CSR(){}

};

