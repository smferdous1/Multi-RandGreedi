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
    
    double startTime = MPI_Wtime(); 
    subMtx.readBin(file);
    // subMtx.verifyCSR();
    double endTime = MPI_Wtime(); 
    if(mpi_rank==0) cout << endTime - startTime <<": finished reading bin file" << endl;
    // cout << subMtx.nRow << " " << subMtx.nCol <<" " << subMtx.nNz << endl;
    file[len+2] = 'd';
    startTime = endTime;
    ReadArray<Size>(file,rowIdxs);
    endTime = MPI_Wtime(); 
    if(mpi_rank==0) cout << endTime - startTime << ": finished reading array" << endl;
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
        
    
    
    for( int i =0; i<level-1; i++){
        if(mpi_rank % firstSib[i] == 0) {
            // cout << mpi_rank << ": Entering selection Loop " << i << " " << subMtx.nRow << " " << subMtx.nNz << " " << subMtx.verPtr.size() << " " << subMtx.verInd.size() << endl;
            
            // if (i!=0){
                // for(Size l: subMtx.verPtr) cout << l<< " ";
                // cout << endl;
                // for(Edge l: subMtx.verInd) cout << l.id << ", " << l.weight << ") (";
                // cout << endl;
            // }
            // subMtx.verifyCSR();
            
            startTime = endTime;
            localOptimizer->select(subMtx, s, k);
            endTime = MPI_Wtime(); 
            if(mpi_rank==0) cout << endTime - startTime << ": finished selection" << endl;
            
            // cout << "Total Coverage of "<< mpi_rank <<"at level" << i << ": " << s.totalGain << endl;
            cout <<mpi_rank <<", " << level-i << ", " << s.totalGain << ", " << MPI_Wtime()-startTime << endl;
            // cout << mpi_rank <<": Input size "<< subMtx.nRow << " k= " << k << " selection size = " << s.selectedRows.size() << endl;
            
            
            // Creating communicator for MPI_Gather
            
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
            
            // Communicator created.
            
            CSR sendMtx;
            
            startTime = MPI_Wtime();
            
           
            subMtx.getSubmatrix(sendMtx, s.selectedRows);
            // sendMtx.verifyCSR();
            // cout << mpi_rank << " " << sendMtx.nRow << " " << sendMtx.nNz << " " << sendMtx.verPtr.size() << " " << sendMtx.verInd.size() << endl;
            // for(Size l: sendMtx.verPtr) cout << l<< " ";
                // cout << endl << endl;
            
            // cout << "Selected rows: " 
            for(Size j = 0; j<s.selectedRows.size(); j++){
                // cout << s.selectedRows[j] << ":";
                s.selectedRows[j] = rowIdxs[s.selectedRows[j]];
                // cout << s.selectedRows[j] << " ";
            }
            // cout << endl;
            
            // cout << mpi_rank << " starting gather:" << endl;
            endTime = MPI_Wtime(); 
            if(mpi_rank==0) cout << endTime - startTime << ": prep to send to parent" << endl;
            startTime = endTime;
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
                // cout << "RowIdxs:" ;
                // for(Size l: rowIdxs) cout << l<< " ";
                // cout << endl;
                // cout << "nnzRecv: "; 
                // for(Size l: nnzRecv) cout << l<< " ";
                // cout << endl;
                // cout << "VerPtr: ";
                // for(Size l: subMtx.verPtr) cout << l<< " ";
                // cout << endl << endl;
                
                // cout << "mem for offset and verInd " << mpi_rank << ":" << endl;
                ResizeVector<int>(&nnzIntRecv,nSiblings);
                ResizeVector<int>(&offsetRecv,nSiblings+1);
                
                offsetRecv[0]=0;
                for(int j=0; j<nSiblings; j++){
                    // if(subMtx.verPtr[j*k]!=(Size)offsetRecv[j]){
                        // cout << "VtxPtr mismatch " << subMtx.verPtr[j*k] << " " << offsetRecv[j] << endl;
                    // }
                    for(int l=1; l<=k; l++){
                        subMtx.verPtr[j*k+l] = subMtx.verPtr[j*k+l] + (Size)offsetRecv[j];
                    }
                    nnzIntRecv[j] = (int)nnzRecv[j];
                    offsetRecv[j+1]=offsetRecv[j]+ nnzIntRecv[j];
                    
                    // cout << offsetRecv[j+1] << " ";
                }
                // cout << endl; 
                subMtx.nNz = (Size)offsetRecv[nSiblings];
                //subMtx.verPtr[subMtx.nRow]= subMtx.nNz;
                // cout << "Gathered Ptr" << endl;
                // for(Size k: subMtx.verPtr) cout << k<< " ";
                // cout << endl;
                ResizeVector<Edge>(&subMtx.verInd,offsetRecv[nSiblings]);
            }
            
            // cout << mpi_rank << " Gathering verInd" << mpi_rank << ":" << subMtx.verInd.size() << endl;
            
            MPI_Gatherv(&(sendMtx.verInd[0]), sendMtx.nNz, mpiEdge, &(subMtx.verInd[0]), &nnzIntRecv[0], &offsetRecv[0], mpiEdge, 0, sibl_comm);
            
            endTime = MPI_Wtime(); 
            if(mpi_rank==0) cout << endTime - startTime << ": Finished gather" << endl;
            
            // if(mpi_rank==myGrp[0]){
                // subMtx.verifyCSR();
                // for(Size r=0; r<subMtx.nRow; r++){
                    // for(Size c=subMtx.verPtr[r]; c<subMtx.verPtr[r+1]-1; c++ ){
                        // cout << "(" << j.id << "," << j.weight << ") ";
                        
                        // cout << subMtx.verInd[c].id << " ";
                        // if(subMtx.verInd[c].id > subMtx.verInd[c+1].id ) cout << "***" << endl;
                    // }
                    // cout << endl;
                // }
                // cout << endl;
            // }
            
            // cout << mpi_rank << " finished Gathering verInd"<< endl;
            
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
        startTime= MPI_Wtime();
        cout << mpi_rank <<", 1, " << s.totalGain << ", " << MPI_Wtime()-startTime << endl;
        
        for(Size j = 0; j<k; j++){
            s.selectedRows[j] = rowIdxs[s.selectedRows[j]];
            //cout << s.selectedRows[j] << " ";
        }
        endTime = MPI_Wtime(); 
            cout << endTime - startTime << ": Final selection" << endl;
    }
    
    return true;
}


