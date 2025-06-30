#include <iostream>
#include <string>
#include <vector>
#include <ctime>

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
  
  /*vector<Size> bMax;
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
  */
  int k= stoi(argv[3]);
  // cout << k << " ";
  CSR g;
  // BMatchingSelection bMatch(bMax, 0.5, k);
<<<<<<< Updated upstream
  // MaxSetCoverSelection bMatch(k);
  KMedoidSelection bMatch(k);
=======
<<<<<<< HEAD
  MaxSetCoverSelection bMatch(k);
  // KMedoidSelection bMatch(k);
=======
  // MaxSetCoverSelection bMatch(k);
  KMedoidSelection bMatch(k);
>>>>>>> 91261662e0ce518ff85162ec70960f43a10228dc
>>>>>>> Stashed changes
  LazyGreedy lz;
  
  cout << argv[1] << " " 
       << argv[2] << " "
       << argv[3] << endl;
  
  cout << "reading file ..." << endl;
  if (!g.readMtx(argv[1])) {
    cout << "file reading error" << endl;
    return 1;
  }
  cout << "Starting" << endl;
  clock_t time_req;
  
  time_req = clock();
  lz.select(g, bMatch, k);
  time_req = clock()-time_req;
  

  
  // cout << "Selected Entries:";
  // for(Size i:bMatch.selectedRows)
    // cout<< i << " ";
  
  cout << "Total Coverage:"<< bMatch.totalGain << endl;
  cout << "Total Time:"<< (float)time_req/CLOCKS_PER_SEC << endl;
  

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
