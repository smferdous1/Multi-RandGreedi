#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <cassert>
#include <cstring>
#include <cmath>


#include "CSR.h"
#include "Utility.h"
#include "Types.h"
using namespace std;

bool CSR::getRow(Size& row, Size edgId) const{
    // performing binary search in verPtr untested
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
            /* inf>>i; 
            inf>>j;

            //j+=lVer; //adjusting for the right hand vertices
            
            if(m1==3) 
                inf>>f; 
            else
                f=drand48()*1000000; */
            
            inf>>i>>j>>f;

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
            for(Size j: SortIndexes(graphCRSIdx[i-1]))
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

bool CSR::getEdgeIncdMtx(CSR& eMtx) const{
    
    vector<Size> eVerPtr;
    ResizeVector<Size>(&eVerPtr,nNz+1);
    vector<Edge> eVerInd;
    ResizeVector<Edge>(&eVerInd,nNz*2);
    
    Size edjCount=0;
    Size nrow=0;
    eMtx.verPtr[0]=0;
    for(Size i=0; i <= nRow; i++){
        //cout << "Adding row " << i << endl;
        for(Size j=verPtr[i];j<verPtr[i+1];j++)
        {
            Size indIdx = eVerPtr[edjCount];
            if (i < verInd[j].id){
                eVerInd[indIdx].id= i;
                eVerInd[indIdx+1].id=verInd[j].id;
            }
            else {
                eVerInd[indIdx+1].id= i;
                eVerInd[indIdx].id=verInd[j].id;
            }
            
            eVerInd[indIdx].weight=verInd[j].weight;
            eVerInd[indIdx+1].weight=verInd[j].weight;
            eVerPtr[edjCount+1]= indIdx+2;
            edjCount++;
        }
    }
    
    eMtx.nRow=nNz;
    eMtx.nCol=(nRow >nCol)?nRow:nCol;
    eMtx.nNz=2*nNz;
    eMtx.maxDeg=2;
    eMtx.verPtr.swap(eVerPtr);
    eMtx.verInd.swap(eVerInd);
    return true;
}

bool CSR::getSimilarityMtx(CSR& sMtx) const{
    
    vector<vector<Size> > graphCRSIdx(nRow);
    vector<vector<Val> > graphCRSVal(nRow);
    
    for(Size i=0; i<nRow; i++){
        for(Size j=i; j < nRow; j++){
            Size jIdx = verPtr[j];
            Size jEnd = verPtr[j+1];
            Val sum = 0;
            for(Size iIdx=verPtr[i];iIdx<verPtr[i+1] && jIdx<jEnd;iIdx++){
                while(verInd[jIdx].id< verInd[iIdx].id && jIdx<jEnd){
                    jIdx++;
                }
                if(verInd[jIdx].id==verInd[iIdx].id){
                    sum+= (verInd[jIdx].weight-verInd[iIdx].weight)*(verInd[jIdx].weight-verInd[iIdx].weight);
                }
            }
            sum = -pow(sum, 0.5);
            graphCRSIdx[i].push_back(j);
            graphCRSVal[i].push_back(sum);
            graphCRSIdx[j].push_back(i);
            graphCRSVal[j].push_back(sum);
        }
    }
    
    vector<Size> sVerPtr;
    ResizeVector<Size>(&sVerPtr,nRow+1);
    vector<Edge> sVerInd;
    ResizeVector<Edge>(&sVerInd,nRow*(nRow-1));
    
    Size count=0;
    sVerPtr[0]=0;
    Size max=0,offset; 
    for(Size i=1;i<=nRow;i++){
        
        offset=graphCRSIdx[i-1].size();
        sVerPtr[i]=sVerPtr[i-1]+offset;
        count=sVerPtr[i-1];
        //cout<<i-1<<" "<<verPtr[i-1]<<" "<<verPtr[i]<<": ";
        for(Size j: SortIndexes(graphCRSIdx[i-1])){
            sVerInd[count].id=graphCRSIdx[i-1][j];
            sVerInd[count].weight=graphCRSVal[i-1][j];
            count++;
        }
        
        if(offset>max)
            max=offset;
    }
    
    assert(count==nRow*(nRow-1));
    sMtx.nRow=nRow;
    sMtx.nCol=nRow;
    sMtx.nNz=nRow*nRow;
    sMtx.maxDeg=max;
    sMtx.verPtr.swap(sVerPtr);
    sMtx.verInd.swap(sVerInd);
    return true;
}

bool CSR::writeBin(char* filename) const {
    
    vector<Size> verId;
    ResizeVector<Size>(&verId, nNz);
    vector<Val> verWt;
    ResizeVector<Val>(&verWt, nNz);
    
    for( Size i=0; i<nNz ;i++){
        verId[i] = verInd[i].id;
        verWt[i] = verInd[i].weight;
    }
    
    ofstream of;
    of.open(filename,ios::out|ios::binary);
    
    if(of.is_open()){
        of.write((char*)&nRow, sizeof(Size));
        of.write((char*)&nCol, sizeof(Size));
        of.write((char*)&nNz, sizeof(Size));
        of.write((char*)&maxDeg, sizeof(Size));
        of.write((char*)&verPtr[0], sizeof(Size) * (nRow+1));
        of.write((char*)&verId[0], sizeof(Size) * nNz);
        of.write((char*)&verWt[0], sizeof(Val) * nNz);
        of.close();
        return true;
    }
    else return false;
}

bool CSR::readBin( char* filename){
    
    ifstream inf;
    inf.open(filename,ios::in|ios::binary);
    if(inf.is_open()){
        inf.read((char*)&nRow, sizeof(Size));
        inf.read((char*)&nCol, sizeof(Size));
        inf.read((char*)&nNz, sizeof(Size));
        inf.read((char*)&maxDeg, sizeof(Size));
        inf.read((char*)&verPtr[0], sizeof(Size) * (nRow+1));
        
        vector<Size> verId;
        ResizeVector<Size>(&verId, nNz);
        vector<Val> verWt;
        ResizeVector<Val>(&verWt, nNz);
        
        ResizeVector<Edge>(&verInd, nNz);
        
        inf.read((char*)&verId[0], sizeof(Size) * nNz);
        inf.read((char*)&verWt[0], sizeof(Val) * nNz);
        inf.close();
        
        for( Size i=0; i<nNz ;i++){
            verInd[i].id = verId[i];
            verInd[i].weight = verWt[i];
        }
        
        return true;
    }
    return false;
}

