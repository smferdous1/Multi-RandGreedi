#include <iostream>
#include <string>
#include <vector>
#include <mpi.h>

#include "CSR.h"
#include "Selection.h"
#include "Optimizer.h"

int main(int argc, char** argv) {
  if (argc != 3) {
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
  
  CSR g;
  MaxSetCoverSelection sCover(k);
  LazyGreedy lg;
  GreeDiL gDl(&lg,b);
  
  cout << "reading file ..." << endl;
  if (!g.readMtx(argv[1])) {
    cout << "file reading error" << endl;
    return 1;
  }
  
  gDl.select(g, sCover, k);
  
  // cout << "Selected Entries:";
  // for(Size i:sCover.selectedRows)
    // cout<< i << " ";
  // cout << "Total Coverage:"<< sCover.totalGain << endl;
  

}