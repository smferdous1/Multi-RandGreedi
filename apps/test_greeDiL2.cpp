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
         << " noOfSelections"
         << " branchingFactor"
         << endl;
    return 1;
  }
  
  
  Size k = stoi(argv[2]);
  int b = stoi(argv[3]);
  
  MPI_Init(&argc, &argv); 
  
  int mpi_rank, mpi_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  pid_t pid = getpid();
  
  // if(mpi_rank==0){
  // cout << argv[1] << " " 
       // << argv[2] << " "
       // << argv[3] << " " 
       // << mpi_size << " " 
       // << pid << endl;
  // }
  CSR g;
  MaxSetCoverSelection sCover(k);
  // KMedoidSelection sCover(k);
  // MaxWtSetCoverSelection sCover(k);
  LazyGreedy lg;
  // GreeDiL2 gDl(&lg,b, argv[1]);
  
  GreeDiL2 gDl2(&lg,b, argv[1]);

  double startTime = MPI_Wtime(); 
  gDl2.select(g, sCover, k);
  double endTime = MPI_Wtime();
  // cout <<  endTime-startTime << endl;
  
  
  // cout << "Selected Entries by " << mpi_rank << " is ";
  // for(Size i:sCover.selectedRows)
    // cout<< i << " ";
  // cout << "Total Gain by " << mpi_rank << " : " << sCover.totalGain << endl;
  // cout << "Total Time by " << mpi_rank << " : " << endTime-startTime << endl;
  
  //smf:need to call this function to get rid of the memories acquired by mpi_init()
  MPI_Finalize();

}