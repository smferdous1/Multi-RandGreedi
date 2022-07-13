#pragma once

#include <vector>
#include "Types.h"

using namespace std;

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
    bool getSimilarity(Val& similarity, Size row1, Size row2) const;
    bool writeBin(char* filename) const;
    bool readBin( char* filename);
    bool verifyCSR() const;
    CSR():nRow(0),nCol(0),nNz(0){}
    ~CSR(){}

};

