#include <iostream>
#include <string>
#include <vector>
#include <mpi.h>

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
  
  int mpi_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

  CSR g;
  MaxSetCoverSelection sCover(k);
  LazyGreedy lg;
  GreeDiL gDl(&lg,b);
  
  cout << "reading file ..." << endl;
  double readTimeS = MPI_Wtime();
  if (!g.readMtx(argv[1])) {
    cout << "file reading error" << endl;
    return 1;
  }
  double readTimeE = MPI_Wtime();
  cout<<"Reading time by " << mpi_rank << "is: "<<readTimeE-readTimeS << endl; 


  double startTime = MPI_Wtime(); 
  gDl.select(g, sCover, k);
  double endTime = MPI_Wtime();
  
  cout << "Selected Entries by " << mpi_rank << "time"<<endTime-startTime<<"is:";
  for(Size i:sCover.selectedRows)
    cout<< i << " ";
  cout << "Total Coverage:"<< sCover.totalGain << endl;
  
  //smf:need to call this function to get rid of the memories acquired by mpi_init()
  MPI_Finalize();
  

}
