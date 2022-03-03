#ifndef MTXREADER_H
#define MTXREADER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>
using namespace std;

struct EdgeE
{
    int head;
    int id;         // Edge tail
    float weight;  // Edge weight
};
struct Edge
{
    int id;         // Edge tail
    float weight;  // Edge weight
};

class CSR
{
    public:
    int nRow;       // number of rows
    int nCol;       // number of columns
    int nNz;      // number of nonzeros
    int maxDeg;
    int* verPtr;    // vertex pointer array of size nVer+1
    Edge* verInd;   // Edge array
    
    bool readMtx(char * filename); // reading as a general graph
    
    CSR():nRow(0),nCol(0),nNz(0),verPtr(NULL),verInd(NULL){}
    ~CSR()
    {
        if(verPtr!=NULL)
            delete verPtr;

        if(verInd!=NULL)
            delete verInd;
    }

};

#endif //MTXREADER_H
