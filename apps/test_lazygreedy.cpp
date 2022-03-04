#include <iostream>
#include <string>
#include <vector>

#include "CSR.h"
#include "Selection.h"
#include "Optimizer.h"

int main(int argc, char** argv) {
  if (argc != 3) {
    cout << "usage: "
         << argv[0]
         << " graphFile"
         << " noOfSelections"
         << endl;
    return 1;
  }
  int k = stoi(argv[2]);
  CSR g;
  MaxSetCoverSelection sCover(k);
  LazyGreedy lz;
  
  cout << "reading file ..." << endl;
  if (!g.readMtx(argv[1])) {
    cout << "file reading error" << endl;
    return 1;
  }
  
  lz.select(g, sCover, k);
  
  cout << "Selected Entries:";
  for(Size i:sCover.selectedRows)
    cout<< i << " ";
  cout << "Total Coverage:"<< sCover.totalGain << endl;
  

}