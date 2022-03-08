#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>     
#include <time.h>
#include <mpi.h>

#include "CSR.h"
#include "Selection.h"
#include "Optimizer.h"

MPI_Datatype mpiSize = MPI_INT;
MPI_Datatype mpiVal = MPI_DOUBLE;

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
    
    vector<Size> rowIdxs, rowIdxSend;
    
    if (rank == 0) {
        srand(time(NULL));
        cout << "Partitioning data:" << endl;
        vector<Size> rowIdxSend;
        ResizeVector<Size>(&rowIdxSend,g.nRow);
        cout << "All rows: ";
        for (int i = 1; i < g.nRow; i++){
            rowIdxSend[i] = rand() % g.nRow ;
            cout << rowIdxSend[i] << " ";
        }
        cout << endl;
    }
    
    ResizeVector<Size>(&rowIdxs,g.nRow/size);
    cout << "Reached Barrier " << endl;
    MPI_Barrier(MPI_COMM_WORLD);
    
    cout << "Sending Row indx" << endl;
    MPI_Scatter(&rowIdxSend[0], g.nRow/size, mpiSize, &rowIdxs[0], g.nRow/size, mpiSize, 0, MPI_COMM_WORLD);
    
    CSR s;
    g.getSubmatrix(s,rowIdxs);
    
    lz.select(s, sCover, k);
      
    cout << "Selected Entries:";
    for(Size i:sCover.selectedRows)
        cout<< rowIdxs[i] << " ";
    cout << "Total Coverage:"<< sCover.totalGain << endl;
    
    
    for(Size i = 0; i<k; i++){
        sCover.selectedRows[i] = rowIdxs[sCover.selectedRows[i]];
        cout << sCover.selectedRows[i] << " ";
    }
    cout << endl;
    
    vector<Size> combinedSoln;
    if(rank == 0)
        ResizeVector<Size>(&combinedSoln,k*size);
    
    MPI_Gather(&combinedSoln[0], g.nRow/size, mpiSize, &sCover.selectedRows[0],g.nRow/size, mpiSize, 0,  MPI_COMM_WORLD);
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    if( rank == 0){
        cout << "Finally selecting from";
        for (int i = 0; i < k*size; i++){
            cout << combinedSoln[i] << " ";
        }
        cout << endl;
        
        MaxSetCoverSelection sCoverCombined(k);
        g.getSubmatrix(s,combinedSoln);
        
        lz.select(s, sCoverCombined, k);
      
        cout << "Final Selected Entries:";
        for(Size i:sCoverCombined.selectedRows)
            cout<< combinedSoln[i] << " ";
        cout << "Total Coverage:"<< sCover.totalGain << endl;
        
    }
    

    MPI_Finalize();                         // terminate MPI
    return 0;
}
