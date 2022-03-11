#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <cassert>
#include <cstring>


#include "CSR.h"
#include "Utility.h"
#include "Types.h"
using namespace std;

bool CSR::getRow(Size& row, Size edgId) const{
    // performing binary search in verPtr
    Size begin =0;
    Size end = nRow-1;
    Size mid = (end + begin)/2;
    if(edgId >= nNz) return false;
    
    while(true){
        if(verPtr[mid]<= edgId && verPtr[mid+1] >edgId){
            row = mid;
            return true;
        }
            
        if(verPtr[mid] < edgId){
            begin = mid+1;
            mid = (end + begin)/2;
        }
        else{
            end = mid-1;
            mid = (end + begin)/2;
        }
    }
    
    
}

bool CSR::readMtx(char* filename){
    Size count=0,i,j;
    Size inp, m1, sym, edgecnt_;
    Size numRow, numCol, nonZeros, numEdges;
    Val f;
    string s;
    ifstream inf;

    inf.open(filename, ios::in);

    if(inf.is_open())
    {
        size_t found1;
        getline(inf,s);
        found1 = s.find("pattern");
        if (found1 != string::npos)
            m1 = 2;
        else
            m1 = 3;
        while(inf.peek()=='%')
            getline(inf,s);
        
 
        inf>>inp;
        numRow=inp;
        inf>>inp;
        numCol=inp;
        inf>>inp;
        nonZeros=inp;

        nRow=numRow;
        nCol=numCol;


        count=inp;
        
        vector<vector<Size> > graphCRSIdx(nRow);
        vector<vector<Val> > graphCRSVal(nRow);
        
        while(count>0) 
        {     
            inf>>i; 
            inf>>j;

            //j+=lVer; //adjusting for the right hand vertices
            
            if(m1==3) 
                inf>>f; 
            else
                f=drand48()*1000000;

            graphCRSIdx[i-1].push_back(j-1); 
            graphCRSVal[i-1].push_back(f);
               
            count--; 
        }     
        inf.close(); 
     
        nNz=nonZeros;
        
        ResizeVector<Size>(&verPtr, nRow+1);
        ResizeVector<Edge>(&verInd, nNz);
        
        verPtr[0]=0;
        Size max=0,offset; 
        for(Size i=1;i<=nRow;i++)
        {
            
            offset=graphCRSIdx[i-1].size();
            verPtr[i]=verPtr[i-1]+offset;
            count=verPtr[i-1];
            //cout<<i-1<<" "<<verPtr[i-1]<<" "<<verPtr[i]<<": ";
            for(Size j=0;j<offset;j++)
            {
                verInd[count].id=graphCRSIdx[i-1][j];
                verInd[count].weight=graphCRSVal[i-1][j];
                count++;

                //cout<<verInd[count-1]<<" ";
            }
            //cout<<endl;
            if(offset>max)
                max=offset;
        }
        
        assert(count==nNz);
        maxDeg=max;

        //cout<<nRow<<" "<<nCol<<" "<<nNz<<endl;

    }
    else return false;

   
   return true;
}

bool CSR::getSubmatrix(CSR& subMtx, vector<Size>& rowIdxs) const {
    
    
    Size nrow = 0;
    Size nnz, count = 0;
    Size max = 0;
    Size offset = 0, currRow=0;
    vector<Size> sVerPtr;
    ResizeVector<Size>(&sVerPtr,rowIdxs.size()+1);
    sVerPtr[nrow] = 0;
    //cout << "Caculating number of Non-zeros "<<endl;
    
    for(Size i:rowIdxs){
        offset = verPtr[i+1]-verPtr[i];
        count+=offset;
        if(offset>max) max=offset;
    }
    
    nnz = count;
    count =0;
    vector<Edge> sVerInd;
    ResizeVector<Edge>(&sVerInd,nnz);
    
    
    for(Size i:rowIdxs){
        //cout << "Adding row " << i << endl;
        for(Size j=verPtr[i];j<verPtr[i+1];j++)
        {
            sVerInd[count].id= verInd[j].id;
            sVerInd[count].weight=verInd[j].weight;
            count++;
        }
        sVerPtr[nrow+1]=count;
        nrow++;
    }
    assert(count==nnz);
    assert(nrow==rowIdxs.size());
    
    subMtx.nRow=nrow;
    subMtx.nCol=nCol;
    subMtx.nNz=nnz;
    subMtx.maxDeg=max;
    subMtx.verPtr.swap(sVerPtr);
    subMtx.verInd.swap(sVerInd);
    
    return true;
}