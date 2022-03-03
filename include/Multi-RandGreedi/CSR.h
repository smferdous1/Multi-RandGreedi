#ifndef CSR_H
#define CSR_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>
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
    vector<Size> verPtr;    // vertex poSizeer array of size nVer+1
    vector<Edge> verInd;   // Edge array
    
    bool readMtx(char * filename); // reading as a general graph
    
    CSR():nRow(0),nCol(0),nNz(0){}
    ~CSR(){}

};

#endif //CSR_H
