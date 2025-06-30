#include <iostream>
#include <string>
#include <vector>
#include <mpi.h>
#include <sys/types.h>
#include <unistd.h>

#include "Utility.h"
#include "CSR.h"
#include "Selection.h"
#include "Optimizer.h"

int main(int argc, char** argv) {
  if (argc != 4) {
    cout << "usage: "
         << argv[0]
         << " graphFile"
         << " solutionFile"
         << " NumberOfCloseNeighbours"
         << endl;
    return 1;
  }
  
  
  Size n = stoi(argv[3]);
  
  
  MPI_Init(&argc, &argv); 
  
  int mpi_rank, mpi_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  pid_t pid = getpid();
  
  CSR g, soln;
  
  Size len = strlen(argv[1]);
    char* file = new char[len+4];
    stpcpy(file, argv[1] );
    file[len]='a'+(mpi_rank/26);
    file[len+1] = 'a'+ (mpi_rank%26);
    file[len+2] = '\0';
    file[len+3] = '\0';
    
    g.readBin(file);
    cout << "Read g ";
    soln.readBin(argv[2]);
    cout << "Read soln";
    
    file[len+2] = 'd';
    vector<Size> rowIdxs;
    ReadArray<Size>(file,rowIdxs);
    cout << "Read Indices" << endl;
    
    
    // vector<vector<pair<Val,Size>>> best(soln.nRow);
    vector<pair<Val,Size>> bTmp;
    //make_heap(bTmp.begin(), bTmp.end(), greater<>{});
    
    for(Size j = 0; j < soln.nRow; j++){
        
        for(Size i = 0; i < g.nRow; i++){
            Size sum = 0;
            Size jIdx = soln.verPtr[j];
                Size jEnd = soln.verPtr[j+1];
            for(Size iIdx=g.verPtr[i];iIdx<g.verPtr[i+1];iIdx++){
                while(jIdx<jEnd && soln.verInd[jIdx].id< g.verInd[iIdx].id ){
                    // cout << verInd[jIdx].weight*verInd[jIdx].weight << "+ ";
                    sum+= soln.verInd[jIdx].weight*soln.verInd[jIdx].weight;
                    jIdx++;
                }
                
                if(jIdx<jEnd && soln.verInd[jIdx].id==g.verInd[iIdx].id){ 
                    // cout  << (verInd[jIdx].weight-verInd[iIdx].weight)*(verInd[jIdx].weight-verInd[iIdx].weight) << "+ ";
                    sum+= (soln.verInd[jIdx].weight-g.verInd[iIdx].weight)*(soln.verInd[jIdx].weight-g.verInd[iIdx].weight);
                    jIdx++;
                }
                else {
                    // cout << verInd[iIdx].weight*verInd[iIdx].weight << "+ ";
                    sum+= g.verInd[iIdx].weight*g.verInd[iIdx].weight;
                }
                
            }
            while(jIdx<jEnd){
                // cout << verInd[jIdx].weight*verInd[jIdx].weight << "+ ";
                sum+= soln.verInd[jIdx].weight*soln.verInd[jIdx].weight;
                jIdx++;
            }
            
            bTmp.push_back(make_pair(sum,rowIdxs[i]));
            // if(bTmp.size()==n)
                // make_heap(bTmp.begin(), bTmp.end());
            
            // if(i>n)
                // pop_heap(bTmp.begin(), bTmp.end());
                // bTmp.pop_back();
            
        }
        sort(bTmp.begin(), bTmp.end());
        for(Size i =0; i<n; i++){
            cout << "(" << bTmp[i].first << "," << bTmp[i].second << ") ";
        }
        bTmp.clear();
        cout << endl;
    }
    
    // for(Size j=0; j < soln.nRow; j++){
        // for(Size i =0; i<n; i++){
            // cout << "(" << bTmp[j][i].first << "," << bTmp[j][i].second << ") ";
        // }
        // cout << endl;
    // }
    
    
  MPI_Finalize();

}