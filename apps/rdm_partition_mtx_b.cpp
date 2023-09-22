#include <iostream>
#include <cstring>
#include <vector>
#include <ctime>
#include <random>
#include <chrono> 

#include <sys/stat.h>
#include <sys/types.h>
#include "CSR.h"
#include "Utility.h"

int main(int argc, char** argv) {
    if (argc != 6) {
        cout << "usage: "
             << argv[0]
             << " graphFilepath"
             << " graphFileName"
             << " LargestNumberOfPartitions"
             << " SmallestNumberOfPartitions"
             << " StepSize"
             << endl;
        return 1;
    }
    
    int end= stoi(argv[3]);
    int start= stoi(argv[4]);
    int step= stoi(argv[5]);
    CSR g;
    char* orgFile= new char[strlen(argv[1])+strlen(argv[2]+1)];
    stpcpy(orgFile,argv[1]);
    strcat(orgFile,argv[2]);
    cout << "reading file ..." << endl;
    if (!g.readMtx(orgFile)) {
        cout << "file reading error" << endl;
        return 1;
    }
    cout << "Starting" << endl;
    clock_t time_req;
  
    g.verifyCSR();
    time_req = clock();
  
    vector<Size> rowIdxSend, rowIdxs;

    srand(time(NULL));
    unsigned seed;
    ResizeVector<Size>(&rowIdxSend,g.nRow);
    
    for( int p=start; p<=end; p+=step){
        char* pathname = new char[4];
        snprintf(pathname, sizeof(pathname), "%03d", p);
        char* fileBase = new char[strlen(argv[1])+strlen(argv[2])+5];
        
        strcpy(fileBase, argv[1]);
        strcat(fileBase,pathname);
        
        if (mkdir(fileBase, 0777) == -1)
            cerr << "Error :  " << strerror(errno) << endl;
        else
            cout << "Directory created";
        
        strcat(fileBase,"/");
        strcat(fileBase, argv[2]);
        
        cout << fileBase << "\" " << endl;
        
            
        seed = std::chrono::system_clock::now().time_since_epoch().count();
        for (Size i = 0; i < g.nRow; i++){
            rowIdxSend[i] = i ;
        }
        shuffle (rowIdxSend.begin(), rowIdxSend.end(), std::default_random_engine(seed));
        
        Size part_size = g.nRow/p;
        Size len = strlen(fileBase);
        char* filename = new char[len+4];
        stpcpy(filename, fileBase);
        filename[len]='a';
        filename[len+1] = 'a';
        filename[len+2] = '\0';
        filename[len+3] = '\0';
        
        CSR subMtx;
        for(Size i =0; i < p ; i++){
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
        
        
        /*srand(time(NULL));
            
        for (Size i = 0; i < g.nRow; i++){
            rowIdxSend[i] = rand() % g.nRow ;
        }
        
        
        filename[len]='A';
        filename[len+1] = 'A';
        filename[len+2] = '\0';
        filename[len+3] = '\0';
        
        for(Size i =0; i < p ; i++){
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
        time_req = clock()-time_req;*/
    }

}

