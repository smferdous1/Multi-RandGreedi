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
    MPI_Datatype types[2] = {mpiSize, mpiVal};
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
    char* file = new char[len+4];
    stpcpy(file, filename );
    file[len]='a'+(mpi_rank/26);
    file[len+1] = 'a'+ (mpi_rank%26);
    file[len+2] = '\0';
    file[len+3] = '\0';
    
    startTime = MPI_Wtime(); 
    subMtx.readBin(file);
    // subMtx.verifyCSR();
    double endTime = MPI_Wtime(); 
    //if(mpi_rank==0) cout << endTime - startTime <<": finished reading bin file" << endl;
    // cout << subMtx.nRow << " " << subMtx.nCol <<" " << subMtx.nNz << endl;
    file[len+2] = 'd';
    startTime = endTime;
    ReadArray<Size>(file,rowIdxs);
    endTime = MPI_Wtime(); 
    //if(mpi_rank==0) cout << endTime - startTime << ": finished reading array" << endl;
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
        
    
    double comm_time=0;
    double other_time=0;
    double proc_time=0;
    double startTmp=0;
    
    MPI_Barrier(MPI_COMM_WORLD);
    startTime= MPI_Wtime();
    
    for( int i =0; i<level-1; i++){
        if(mpi_rank % firstSib[i] == 0) {
            
            // Greedy selction of local data
            startTmp = MPI_Wtime();
            localOptimizer->select(subMtx, s, k);
            proc_time+= MPI_Wtime() - startTmp;
            
            
            
            // Creating communicator for MPI_Gather
            startTmp = MPI_Wtime();
            
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
            
            MPI_Group sibl_group;
            MPI_Group_incl(world_group, nSiblings, &myGrp[0], &sibl_group);
            
            MPI_Comm sibl_comm;
            MPI_Comm_create_group(MPI_COMM_WORLD, sibl_group, 0, &sibl_comm);
            other_time = MPI_Wtime()- startTmp;
            // Communicator created.
            
            
            //Communication Begins
            startTmp= MPI_Wtime();
            
            CSR sendMtx;
            subMtx.getSubmatrix(sendMtx, s.selectedRows);
            
            for(Size j = 0; j<s.selectedRows.size(); j++){
                s.selectedRows[j] = rowIdxs[s.selectedRows[j]];
            }
            
            
            if(mpi_rank == myGrp[0]){
                ResizeVector<Size>(&rowIdxs,k*nSiblings);
                
                ResizeVector<Size>(&nnzRecv,nSiblings);
                
                subMtx.nRow = k*nSiblings;
                
                ResizeVector<Size>(&subMtx.verPtr,k*nSiblings+1);
                subMtx.verPtr[0]=0;
            }
            
            MPI_Gather(&(s.selectedRows[0]), k, mpiSize, &rowIdxs[0],k, mpiSize, 0, sibl_comm);
            
            MPI_Gather(&(sendMtx.nNz), 1, mpiSize, &nnzRecv[0],1, mpiSize, 0, sibl_comm);
            
            
            MPI_Gather(&(sendMtx.verPtr[1]), k, mpiSize, &subMtx.verPtr[1], k, mpiSize, 0, sibl_comm);
            
            
            if(mpi_rank==myGrp[0]){
                ResizeVector<int>(&nnzIntRecv,nSiblings);
                ResizeVector<int>(&offsetRecv,nSiblings+1);
                
                offsetRecv[0]=0;
                for(int j=0; j<nSiblings; j++){
                    
                    for(int l=1; l<=k; l++){
                        subMtx.verPtr[j*k+l] = subMtx.verPtr[j*k+l] + (Size)offsetRecv[j];
                    }
                    nnzIntRecv[j] = (int)nnzRecv[j];
                    offsetRecv[j+1]=offsetRecv[j]+ nnzIntRecv[j];
                   
                }
                subMtx.nNz = (Size)offsetRecv[nSiblings];
                ResizeVector<Edge>(&subMtx.verInd,offsetRecv[nSiblings]);
            }
            
            
            MPI_Gatherv(&(sendMtx.verInd[0]), sendMtx.nNz, mpiEdge, &(subMtx.verInd[0]), &nnzIntRecv[0], &offsetRecv[0], mpiEdge, 0, sibl_comm);
            
            comm_time+= MPI_Wtime() - startTmp;
            // Communication ends
            
        }
    }
    
    if(mpi_rank == 0){
        
        // Selection at root
        startTmp = MPI_Wtime();
        localOptimizer->select(subMtx, s, k);
        proc_time+= MPI_Wtime()-startTmp;
      
        // Getting original indices
        startTmp = MPI_Wtime();
        for(Size j = 0; j<k; j++){
            s.selectedRows[j] = rowIdxs[s.selectedRows[j]];
        }
        other_time+=MPI_Wtime()-startTmp; 
        endTime = MPI_Wtime();
        
        cout << k << ", " << b << ", " << mpi_size << ", " << endTime - startTime << ", " << comm_time << ", " << proc_time <<  ", " <<other_time << endl;
        
    }
    
    return true;
}


