#include <iostream>
#include <cstring>
#include <vector>
#include <ctime>
#include <random>
#include <chrono> 

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
  
    g.verifyCSR();
    time_req = clock();
  
    vector<Size> rowIdxSend, rowIdxs;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    
    ResizeVector<Size>(&rowIdxSend,g.nRow);
        
    for (Size i = 0; i < g.nRow; i++){
        rowIdxSend[i] = i ;
    }
    shuffle (rowIdxSend.begin(), rowIdxSend.end(), std::default_random_engine(seed));
    
    
    Size part_size = g.nRow/k;
    Size len = strlen(argv[1]);
    char* filename = new char[len+4];
    stpcpy(filename, argv[1]);
    filename[len]='a';
    filename[len+1] = 'a';
    filename[len+2] = '\0';
    filename[len+3] = '\0';
    
    CSR subMtx;
    for(Size i =0; i < k ; i++){
        rowIdxs = std::vector<Size>(rowIdxSend.begin() + i* part_size, rowIdxSend.begin() + (i+1)*part_size);
        g.getSubmatrix(subMtx,rowIdxs);
        subMtx.verifyCSR();
        cout << "Verified";
        filename[len] = 'a'+(i/26);
        filename[len+1] = 'a'+(i%26);
        filename[len+2] = '\0';
        subMtx.writeBin(filename);
        cout << filename << endl;
        filename[len+2] = 'd';
        WriteArray<Size>(filename,rowIdxs);
    }
    time_req = clock()-time_req;
    
    
    srand(time(NULL));
        
    for (Size i = 0; i < g.nRow; i++){
        rowIdxSend[i] = rand() % g.nRow ;
    }
    
    
    filename[len]='A';
    filename[len+1] = 'A';
    filename[len+2] = '\0';
    filename[len+3] = '\0';
    
    for(Size i =0; i < k ; i++){
        rowIdxs = std::vector<Size>(rowIdxSend.begin() + i* part_size, rowIdxSend.begin() + (i+1)*part_size);
        g.getSubmatrix(subMtx,rowIdxs);
        subMtx.verifyCSR();
        cout << "Verified";
        filename[len] = 'A'+(i/26);
        filename[len+1] = 'A'+(i%26);
        filename[len+2] = '\0';
        subMtx.writeBin(filename);
        cout << filename << endl;
        filename[len+2] = 'd';
        WriteArray<Size>(filename,rowIdxs);
    }
    time_req = clock()-time_req;

}

