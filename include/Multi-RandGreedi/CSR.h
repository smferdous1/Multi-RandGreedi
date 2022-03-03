#ifndef MTXREADER_H
#define MTXREADER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>
using namespace std;

struct EdgeE {
    int head;
    int id;         // Edge tail
    float weight;  // Edge weight
};
struct Edge {
    int id;         // Edge tail
    double weight;  // Edge weight
};

class CSR {
    public:
    int nRow;       // number of rows
    int nCol;       // number of columns
    int nNz;      // number of nonzeros
    int maxDeg;
    vector<int> verPtr;    // vertex pointer array of size nVer+1
    vector<Edge> verInd;   // Edge array
    
    bool readMtx(char * filename); // reading as a general graph
    
    CSR():nRow(0),nCol(0),nNz(0){}
    ~CSR(){}

};

#endif //MTXREADER_H
