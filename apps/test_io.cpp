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
  
  CSR g
  
  cout << "reading graph file ..." << endl;
  if (!g.readMtxG(argv[1])) {
    cout << "graph file reading error" << endl;
    return 1;
  }
  
  cout << "{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{" << endl;
  cout << "Graph (begin)" << endl;
  cout << "number of vertices: " << g->nVer << endl;
  cout << "number of edges: " << g->nEdge << endl;
  cout << "Graph (end)" << endl;
  cout << "}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}" << endl;

