#pragma once

#include <vector>
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
    bool getEdgeIncdMtx(CSR& eMtx) const;
    bool getSimilarityMtx(CSR& sMtx) const;
    bool writeBin(char* filename) const;
    bool readBin( char* filename);
    CSR():nRow(0),nCol(0),nNz(0){}
    ~CSR(){}

};

