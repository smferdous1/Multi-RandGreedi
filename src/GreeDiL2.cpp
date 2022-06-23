#include <mpi.h>


#include "Optimizer.h"
#include "Utility.h"
#include "MPI_Types.h"

using namespace std;


bool GreeDiL2::select(const CSR& g, Selection& s,Size k){
    
    // cout << "Starting select" << endl;
    int mpi_rank, mpi_size;
    double startTime = MPI_Wtime();
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size); 
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    
    MPI_Group world_group;
    MPI_Comm_group(MPI_COMM_WORLD, &world_group);
    
    const int nitems=2;
    int blocklengths[2] = {1,1};
    MPI_Datatype types[2] = {MPI_UNSIGNED_LONG, MPI_DOUBLE};
    MPI_Datatype mpiEdge;

    MPI_Aint     offsets[2];
    offsets[0] = offsetof(Edge, id);
    offsets[1] = offsetof(Edge, weight);

    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpiEdge);
    MPI_Type_commit(&mpiEdge);
    
    //______________________________________
    // cout << "Finished init" << endl;
    
    Size b = branchingFactor;
    vector<Size> nnzRecv, rowIdxs ;
    vector<int> nnzIntRecv, offsetRecv;
    CSR subMtx;
    
    Size len = strlen(filename);
    char* file = new char[len+3];
    stpcpy(file, filename );
    file[len]='0'+mpi_rank;
    file[len+1] = '\0';
    file[len+2] = '\0';
    
    
    subMtx.readBin(file);
    // cout << mpi_rank <<": finished reading bin file" << endl;
    // cout << subMtx.nRow << " " << subMtx.nCol <<" " << subMtx.nNz << endl;
    file[len+1] = 'd';
    ReadArray<Size>(file,rowIdxs);
    // cout << mpi_rank << ": finished reading array" << endl;
    // for( Size i:rowIdxs){ cout << i << " ";}
    // cout << endl;
    
    int count = mpi_size-1;
    Size level = 1;
    while(count!=0){
        count/= b;
        level++;
    }
    
    vector<Size> firstSib;
    ResizeVector<Size>(&firstSib, level+1);
    firstSib[0]=1;
    for( int i = 1; i<=level; i++)
        firstSib[i]= firstSib[i-1]*b;
        
    
    cout << "Entering selection Loop" << endl;
    for( int i =0; i<level-1; i++){
        if(mpi_rank % firstSib[i] == 0) {
            
            localOptimizer->select(subMtx, s, k);
            
            // cout << "Total Coverage of "<< mpi_rank <<"at level" << i << ": " << s.totalGain << endl;
            cout <<mpi_rank <<", " << level-i << ", " << s.totalGain << ", " << MPI_Wtime()-startTime << endl;
            cout << mpi_rank <<": Input size "<< subMtx.nRow << " k= " << k << " selection size = " << s.selectedRows.size() << endl;
            
            
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
            
            CSR sendMtx;
            
            subMtx.getSubmatrix(sendMtx, s.selectedRows);
            
            
            for(Size j = 0; j<s.selectedRows.size(); j++){
                // cout << s.selectedRows[j] << ":";
                s.selectedRows[j] = rowIdxs[s.selectedRows[j]];
                // cout << s.selectedRows[j] << " ";
            }
            //cout << endl;
            
            // cout << "starting gather at " << mpi_rank << ":" << endl;
            if(mpi_rank == myGrp[0]){
                ResizeVector<Size>(&rowIdxs,k*nSiblings);
                
                ResizeVector<Size>(&nnzRecv,nSiblings);
                
                subMtx.nRow = k*nSiblings;
                
                ResizeVector<Size>(&subMtx.verPtr,k*nSiblings);
            }
            
            // cout << "Gathering row idx" << mpi_rank << ":" << endl;
            MPI_Gather(&(s.selectedRows[0]), k, mpiSize, &rowIdxs[0],k, mpiSize, 0, sibl_comm);
            
            // cout << "Gathering nNz" << mpi_rank << ":" << endl;
            MPI_Gather(&(sendMtx.nNz), 1, mpiSize, &nnzRecv[0],1, mpiSize, 0, sibl_comm);
            
            
            // cout << "Gathering verPtr" << mpi_rank << ":" << endl;
            MPI_Gather(&(sendMtx.verPtr[1]), k, mpiSize, &subMtx.verPtr[0], k, mpiSize, 0, sibl_comm);
            
            if(mpi_rank==myGrp[0]){
                cout << "mem for offset and verInd" << mpi_rank << ":" << endl;
                ResizeVector<int>(&nnzIntRecv,nSiblings);
                ResizeVector<int>(&offsetRecv,nSiblings+1);
                
                offsetRecv[0]=0;
                for(int j=0; j<nSiblings; j++){
                    for(int l=0; l<k; l++){
                        subMtx.verPtr[j*k+l] = subMtx.verPtr[j*k+l] + offsetRecv[j];
                    }
                    nnzIntRecv[j] = (int)nnzRecv[j];
                    offsetRecv[j+1]=offsetRecv[j]+ nnzIntRecv[j];
                    cout << offsetRecv[j+1] << " ";
                }
                cout << endl; 
                subMtx.nNz = offsetRecv[nSiblings];
                ResizeVector<Edge>(&subMtx.verInd,offsetRecv[nSiblings]);
            }
            
            cout << "Gathering verInd" << mpi_rank << ":" << subMtx.verInd.size() << endl;
            
            MPI_Gatherv(&(sendMtx.verInd[0]), sendMtx.nNz, mpiEdge, &subMtx.verInd[0], &nnzIntRecv[0], &offsetRecv[0], mpiSize, 0, sibl_comm);
            
            cout << "finished Gathering verInd"<< endl;
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
        localOptimizer->select(subMtx, s, k);
      
        /* cout << "Final Selected Entries:";
        for(Size i:s.selectedRows)
            cout<< rowIdxs[i] << " ";
        cout << "Total Coverage:"<< s.totalGain << endl; */
        // cout << mpi_rank <<", 1, " << s.totalGain << ", " << MPI_Wtime()-startTime << endl;
        
        for(Size j = 0; j<k; j++){
            s.selectedRows[j] = rowIdxs[s.selectedRows[j]];
            //cout << s.selectedRows[j] << " ";
        }
    }
    
    return true;
}


