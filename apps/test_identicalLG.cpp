#include <iostream>
#include <string>
#include <vector>
#include <mpi.h>

#include "CSR.h"
#include "Selection.h"
#include "Optimizer.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        cout << "usage: "
             << argv[0]
             << " graphFile"
             << " noOfSelections"
             //<< " number of machines"
             << endl;
        return 1;
    }
    int k = stoi(argv[2]);
    //int m = stoi(argv[3]);
      
  

    // MPI Parameters
    int rank, size, len;
    
    MPI_Init(&argc, &argv);  
    MPI_Comm_size(MPI_COMM_WORLD, &size); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    
    CSR g;
    MaxSetCoverSelection sCover(k);
    LazyGreedy lz;
    
    cout << "reading file ..." << endl;
    if (!g.readMtx(argv[1])) {
        cout << "file reading error" << endl;
        return 1;
    }
    
    vector<Size> rowIdxs;
    ResizeVector<Size>(&rowIdxs,g.nRow/size);
    
    srand(time(NULL));
    for(Size i=0; i< g.nRow/size; i++){
        rowIdxs[i] = rand() % g.nRow ;
        cout << rowIdxs[i];
    }
    cout << endl;
    
    CSR s;
    g.getSubmatrix(s,rowIdxs);
    
    lz.select(g, sCover, k);
      
    cout << "Selected Entries:";
    for(Size i:sCover.selectedRows)
        cout<< i << " ";
    cout << "Total Coverage:"<< sCover.totalGain << endl;

    MPI_Finalize();                         // terminate MPI
    return 0;
}
