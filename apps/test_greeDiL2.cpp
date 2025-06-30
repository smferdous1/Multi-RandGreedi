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
<<<<<<< Updated upstream
=======
<<<<<<< HEAD
  // std::ofstream out;
  // out.open(argv[4], std::ios_base::app);
  // std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
  // std::cout.rdbuf(out.rdbuf());
=======
>>>>>>> Stashed changes
  std::ofstream out;
  out.open(argv[4], std::ios_base::app);
  std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
  std::cout.rdbuf(out.rdbuf());
<<<<<<< Updated upstream
=======
>>>>>>> 91261662e0ce518ff85162ec70960f43a10228dc
>>>>>>> Stashed changes
  
  
  double startTime = MPI_Wtime(); 
  gDl2.select(g, sCover, k);
  double endTime = MPI_Wtime();
<<<<<<< Updated upstream
  
  //reset to standard output again
  std::cout.rdbuf(coutbuf);
=======
<<<<<<< HEAD
  // for(int i=0; i < k ; i++){
  //     cout << sCover.selectedRows[i] << " ";
  // }
  // cout << endl;
  //reset to standard output again
  // std::cout.rdbuf(coutbuf);
=======
  
  //reset to standard output again
  std::cout.rdbuf(coutbuf);
>>>>>>> 91261662e0ce518ff85162ec70960f43a10228dc
>>>>>>> Stashed changes
  
  
  //smf:need to call this function to get rid of the memories acquired by mpi_init()
  MPI_Finalize();

}