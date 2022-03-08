#include <mpi.h>


#include "Optimizer.h"
#include "Utility.h"
#include "MPI_Types.h"
using namespace std;


bool GreeDiL::select(const CSR& g, Selection& s,Size k){
    
    int mpi_rank, mpi_size;
    
    MPI_Init(&argc, &argv);  
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size); 
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    
    MPI_Group world_group;
    MPI_Comm_group(MPI_COMM_WORLD, &world_group);
    
    
    assert(g.nRow <= machCapacity* mpi_size);
    
    vector<Size> rowIdxs, rowIdxSend;
    
    if (mpi_rank == 0) {
        srand(time(NULL));
        ResizeVector<Size>(&rowIdxSend,g.nRow);
        cout << "Partitioning data:" << endl;
        
        for (Size i = 0; i < g.nRow; i++){
            rowIdxSend[i] = rand() % g.nRow ;
        }
    }
    
    ResizeVector<Size>(&rowIdxs,g.nRow/mpi_size);
    
    MPI_Scatter(&rowIdxSend[0], g.nRow/mpi_size, mpiSize, &rowIdxs[0], g.nRow/mpi_size, mpiSize, 0, MPI_COMM_WORLD);
    
    Size b = machCapacity/k;
    Size count = mpi_size-1;
    Size level = 1;

    while(count!=0){
        count/= b;
        level++;
    }
    
    // firstSib[i] is the first sibling of 0 at level i 
    vector<Size> firstSib;
    ResizeVector<Size>(&firstSib, level);
    firstSib.push_back(1);
    for( int i = 1; i<=level; i++)
        firstSib[i]= firstSib[i-1]*b;
        
    
    
    for( int i =0; i<level; i++){
        if(mpi_rank % firstSib[i] == 0) {
            CSR s;
            g.getSubmatrix(s,rowIdxs);
            
            lz.select(s, sCover, k);
            
            cout << "Total Coverage of "<< mpi_rank << ": " << sCover.totalGain << endl;
    
    
            for(Size j = 0; j<k; j++){
                sCover.selectedRows[j] = rowIdxs[sCover.selectedRows[j]];
                //cout << sCover.selectedRows[j] << " ";
            }
            //cout << endl;
            
            
            // Creating communicator for MPI_Gather
            
            vector<Size> myGrp;
            Size nSiblings;
            ResizeVector<Size>(&myGrp, b);
            
            
            myGrp[0] = (mpi_rank/ firstSib[i+1])* firstSib[i+1]; 
            for(int nSiblings = 0; nSiblings < b-1 ; j++){
                if(myGrp[j]+firstSib[i]<mpi_size)
                    myGrp[j+1] = myGrp[j]+firstSib[i];
                else
                    break;
            }
            nSiblings++;
            
            MPI_Group sibl_group;
            MPI_Group_incl(world_group, nSiblings, &myGrp[0], &sibl_group);
            
            MPI_Comm sibl_comm;
            MPI_Comm_create_group(MPI_COMM_WORLD, sibl_group, 0, &sibl_comm);
            
            // Communicator created.
            
            if(mpi_rank== myGrp[0])
                ResizeVector<Size>(&rowIdxs,k*nSiblings);
            
            MPI_Gather(&sCover.selectedRows[0], k, mpiSize, &rowIdxs[0],k, mpiSize, 0, sibl_comm);
        }
    }
    
    if(mpi_rank == 0){
        cout << "Finally selecting from";
        for (Size i = 0; i < k*size; i++){
            cout << rowIdxs[i] << " ";
        }
        cout << endl;
        
        MaxSetCoverSelection sCoverCombined(k);
        g.getSubmatrix(s,rowIdxs);
        
        lz.select(s, sCoverCombined, k);
      
        cout << "Final Selected Entries:";
        for(Size i:sCoverCombined.selectedRows)
            cout<< rowIdxs[i] << " ";
        cout << "Total Coverage:"<< sCover.totalGain << endl;
    }
    
    return true;
}


