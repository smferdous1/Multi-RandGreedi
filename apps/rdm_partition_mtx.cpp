#include <iostream>
#include <cstring>
#include <vector>
#include <ctime>

#include "CSR.h"
#include "Utility.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        cout << "usage: "
             << argv[0]
             << " graphFile"
             << " numberOfPartitions"
             << endl;
        return 1;
    }
    
    int k= stoi(argv[2]);
    CSR g;
  
    cout << "reading file ..." << endl;
    if (!g.readMtx(argv[1])) {
        cout << "file reading error" << endl;
        return 1;
    }
    cout << "Starting" << endl;
    clock_t time_req;
  
    
    time_req = clock();
  
    vector<Size> rowIdxSend, rowIdxs;

    srand(time(NULL));
    ResizeVector<Size>(&rowIdxSend,g.nRow);
        
    for (Size i = 0; i < g.nRow; i++){
        rowIdxSend[i] = rand() % g.nRow ;
    }
    
    Size part_size = g.nRow/k;
    Size len = strlen(argv[1]);
    char* filename = new char[len+3];
    stpcpy(filename, argv[1]);
    filename[len]='0';
    filename[len+1] = '\0';
    filename[len+2] = '\0';
    
    CSR subMtx;
    for(Size i =0; i < k ; i++){
        rowIdxs = std::vector<Size>(rowIdxSend.begin() + i* part_size, rowIdxSend.begin() + (i+1)*part_size);
        g.getSubmatrix(subMtx,rowIdxs);
        subMtx.verifyCSR();
        cout << "Verified";
        filename[len] = '0'+i;
        filename[len+1] = '\0';
        subMtx.writeBin(filename);
        cout << filename << endl;
        filename[len+1] = 'd';
        WriteArray<Size>(filename,rowIdxs);
    }
    time_req = clock()-time_req;

}

