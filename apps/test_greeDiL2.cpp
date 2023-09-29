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
  if (argc != 5) {
    cout << "usage: "
         << argv[0]
         << " graphFile"
         << " noOfSelections"
         << " branchingFactor"
         << " outputCSVfile"
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
  
  CSR g;
  MaxSetCoverSelection sCover(k);
  // KMedoidSelection sCover(k);
  // MaxWtSetCoverSelection sCover(k);
  LazyGreedy lg;
  // GreeDiL2 gDl(&lg,b, argv[1]);
  
  GreeDiL2 gDl2(&lg,b, argv[1]);

  
  // redirecting cout to csv
  std::ofstream out;
  out.open(argv[4], std::ios_base::app);
  std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
  std::cout.rdbuf(out.rdbuf());
  
  
  double startTime = MPI_Wtime(); 
  gDl2.select(g, sCover, k);
  double endTime = MPI_Wtime();
  
  //reset to standard output again
  std::cout.rdbuf(coutbuf);
  
  
  //smf:need to call this function to get rid of the memories acquired by mpi_init()
  MPI_Finalize();

}