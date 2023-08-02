#include <iostream>
#include <cstring>
#include <vector>
#include <ctime>

#include "CSR.h"
#include "Utility.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "usage: "
             << argv[0]
             << " graphFile"
             << endl;
        return 1;
    }
    
    CSR g;
    clock_t time_req;
 
    
    time_req = clock();
    cout << "reading file ..." << endl;
    if (!g.readBin(argv[1])) {
        cout << "file reading error" << endl;
        return 1;
    }
    cout << "Time:"<< clock()-time_req << endl;
    
}