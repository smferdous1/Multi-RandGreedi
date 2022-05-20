#include <mpi.h>


#include "Optimizer.h"
#include "Utility.h"
#include "MPI_Types.h"

using namespace std;


bool GreeDiL::select(const CSR& g, Selection& s,Size k){
    
    int mpi_rank, mpi_size;
    double startTime = MPI_Wtime();
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size); 
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    
    MPI_Group world_group;
    MPI_Comm_group(MPI_COMM_WORLD, &world_group);
    
    Size b = branchingFactor;
    
    // b = machCapacity/k;
    // assert(g.nRow <= machCapacity* mpi_size);
    
    vector<Size> rowIdxs, rowIdxSend;
    
    if (mpi_rank == 0) {
        srand(time(NULL));
        ResizeVector<Size>(&rowIdxSend,g.nRow);
        // cout << "Partitioning data:" << endl;
        
        for (Size i = 0; i < g.nRow; i++){
            rowIdxSend[i] = rand() % g.nRow ;
        }
    }
    
    ResizeVector<Size>(&rowIdxs,g.nRow/mpi_size);
    
    MPI_Scatter(&rowIdxSend[0], g.nRow/mpi_size, mpiSize, &rowIdxs[0], g.nRow/mpi_size, mpiSize, 0, MPI_COMM_WORLD);
    
    /* cout << " My data :";
    for( Size i : rowIdxs)
        cout << i << " ";
    cout << endl;
     */
    
    Size count = mpi_size-1;
    Size level = 1;
    // cout << "Calculating number Levels" << endl;
    while(count!=0){
        count/= b;
        level++;
    }
    
    // firstSib[i] is the first sibling of 0 at level i 
    // cout << "Calculating First sibling at each level" << endl;
    vector<Size> firstSib;
    ResizeVector<Size>(&firstSib, level+1);
    firstSib[0]=1;
    for( int i = 1; i<=level; i++)
        firstSib[i]= firstSib[i-1]*b;
        
    
    // cout << "Entering selection Loop" << endl;
    for( int i =0; i<level-1; i++){
        if(mpi_rank % firstSib[i] == 0) {
            CSR subMtx;
            g.getSubmatrix(subMtx,rowIdxs);
            
            localOptimizer->select(subMtx, s, k);
            
            // cout << "Total Coverage of "<< mpi_rank <<"at level" << i << ": " << s.totalGain << endl;
            // cout <<mpi_rank <<", " << level-i << ", " << s.totalGain << ", " << MPI_Wtime()-startTime << endl;
            cout << mpi_rank <<": Input size "<< subMtx.nRow << " k= " << k << " selection size = " << s.selectedRows.size() << endl;
            
            
            
            for(Size j = 0; j<s.selectedRows.size(); j++){
                // cout << s.selectedRows[j] << ":";
                s.selectedRows[j] = rowIdxs[s.selectedRows[j]];
                // cout << s.selectedRows[j] << " ";
            }
            //cout << endl;
            
            
            // Creating communicator for MPI_Gather
            // cout << "Creating Communicator Group" << endl;
            vector<int> myGrp;
            int nSiblings;
            ResizeVector<int>(&myGrp, b);
            
            
            myGrp[0] = (mpi_rank/ firstSib[i+1])* firstSib[i+1]; 
            for(nSiblings = 0; nSiblings < b-1 ; nSiblings++){
                if(myGrp[nSiblings]+firstSib[i]<mpi_size)
                    myGrp[nSiblings+1] = myGrp[nSiblings]+firstSib[i];
                else
                    break;
            }
            nSiblings++;
            
            // cout << mpi_rank << " Group " << nSiblings << " sib :";
            // for(int j:myGrp){
                // cout << j << " ";
            // }cout << endl;
            
            
            MPI_Group sibl_group;
            MPI_Group_incl(world_group, nSiblings, &myGrp[0], &sibl_group);
            
            MPI_Comm sibl_comm;
            MPI_Comm_create_group(MPI_COMM_WORLD, sibl_group, 0, &sibl_comm);
            
            // Communicator created.
            // cout << "starting gather at " << mpi_rank << ":" << endl;
            if(mpi_rank == myGrp[0]){
                // cout << mpi_rank << "is root at level " << i << endl;
                ResizeVector<Size>(&rowIdxs,k*nSiblings);
            }
            
            MPI_Gather(&(s.selectedRows[0]), k, mpiSize, &rowIdxs[0],k, mpiSize, 0, sibl_comm);
            
            /* if(mpi_rank == myGrp[0]){
                for(Size i : rowIdxs)
                    cout << i << " ";
                cout << endl;
                }
             */
            
        }
    }
    
    if(mpi_rank == 0){
        /* cout << "Finally selecting from";
        for (Size i : rowIdxs){
            cout << i << " ";
        }
        cout << endl;
         */
        CSR subMtx;
        g.getSubmatrix(subMtx,rowIdxs);
        localOptimizer->select(subMtx, s, k);
      
        /* cout << "Final Selected Entries:";
        for(Size i:s.selectedRows)
            cout<< rowIdxs[i] << " ";
        cout << "Total Coverage:"<< s.totalGain << endl; */
        cout << mpi_rank <<", 1, " << s.totalGain << ", " << MPI_Wtime()-startTime << endl;
        
        for(Size j = 0; j<k; j++){
            s.selectedRows[j] = rowIdxs[s.selectedRows[j]];
            //cout << s.selectedRows[j] << " ";
        }
    }
    
    return true;
}


