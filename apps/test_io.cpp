#include <iostream>
#include <string>
#include <vector>
#include <cassert>

#include "mtxReader.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    cout << "usage: "
         << argv[0]
         << " graphFile"
         << endl;
    return 1;
  }
  
  CSR g;
  
  cout << "reading graph file ..." << endl;
  if (!g.readMtx(argv[1])) {
    cout << "graph file reading error" << endl;
    return 1;
  }
  
  cout << "{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{" << endl;
  cout << "Data (begin)" << endl;
  cout << "number of entries: " << g.nRow << endl;
  cout << "number of features: " << g.nCol << endl;
  cout << "number of Non-Zeros: " << g.nNz << endl;
  cout << "Graph (end)" << endl;
  cout << "}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}" << endl;

}