#include <iostream>
#include <string>
#include <vector>

#include "CSR.h"
#include "Selection.h"
#include "Optimizer.h"

int main(int argc, char** argv) {
  if (argc != 4) {
    cout << "usage: "
         << argv[0]
         << " graphFile"
         << " bvalueFile"
         << " noOfSelections"
         << endl;
    return 1;
  }
  
  vector<Size> bMax;
  Size len;
  
    ifstream inf;
    inf.open(argv[2], ios::in);
    
    if(inf.is_open())
    {
        cout << "Reading b file" << endl;
        inf >> len;
        ResizeVector<Size>(&bMax, len);
        for(Size i = 0; i<len ; i++){
            inf >> bMax[i];
        }
    }
    else {
        cout << "b file unopened" << endl;
        return 1;
    }
  
  int k= stoi(argv[3]);
  
  CSR g;
  BMatchingSelection bMatch(bMax, 0.5, k);
  LazyGreedy lz;
  
  cout << "reading file ..." << endl;
  if (!g.readMtx(argv[1])) {
    cout << "file reading error" << endl;
    return 1;
  }
  
  lz.select(g, bMatch, k);
  
  cout << "Selected Entries:";
  for(Size i:bMatch.selectedRows)
    cout<< i << " ";
  cout << "Total Coverage:"<< bMatch.totalGain << endl;
  

}

/* bool readVec(vector<Size>& vec, char* filename){
    
    Size len;
    
    ifstream inf;
    inf.open(filename, ios::in);
    
    
    
    if(inf.is_open())
    {
        inf >> len;
        ResizeVector<Size>(&vec, len);
        
        for(Size i = 0; i<len ; i++){
            inf >> vec[i];
        }
        return true;
    }
    return false;
} */