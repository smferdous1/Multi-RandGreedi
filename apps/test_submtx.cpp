#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <stdlib.h>
#include <time.h>

#include "CSR.h"
#include "Utility.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        cout << "usage: "
             << argv[0]
             << " graphFile"
             << " nRowsOfSubmatrix"
             << endl;
        return 1;
    }
  
    CSR g;
    
    cout << "reading graph file ..." << endl;
    if (!g.readMtx(argv[1])) {
        cout << "graph file reading error" << endl;
        return 1;    
    }
    Size sNrow = stoi(argv[2]);
    vector<Size> rowIdxs;
    ResizeVector<Size>(&rowIdxs,sNrow);
    
    srand(time(NULL));
    cout << "selecting random rows" << endl;
    for(Size i=0; i< sNrow; i++){
        rowIdxs[i] = rand() % g.nRow ;
        cout << rowIdxs[i];
    }
    cout << endl;
    
    CSR s;
    g.getSubmatrix(s,rowIdxs);
    
    cout << "{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{" << endl;
    cout << "Data (begin)" << endl;
    cout << "number of entries: " << s.nRow << endl;
    cout << "number of features: " << g.nCol << endl;
    cout << "number of Non-Zeros: " << s.nNz << endl;
    for(Size i:s.verPtr)
        cout<< i << " ";
    cout << endl;
    for(Edge i:s.verInd)
        cout<< i.id <<" " << i.weight << " " << endl;
    cout << endl;
  
    cout << "Data (end)" << endl;
    cout << "}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}" << endl;

}