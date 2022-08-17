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
            
            //smf:this should be faster than reading one by one. But don't quote me on that!  
            if (m1==2)
            {
                inf>>i>>j;
                f =drand48()*1000000;
            }
            else 
                inf>>i>>j>>f;
            
            /* inf>>i; 
            inf>>j;

            //j+=lVer; //adjusting for the right hand vertices
            
            if(m1==3) 
                inf>>f; 
            else
                f=drand48()*1000000; */
            

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
            
            
            // Sort indices? 
            for(Size j: SortIndexes(graphCRSIdx[i-1])){
                
            // for(Size j=0;j<offset;j++){
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
    
    // cout << " Verifying the input Matrix" << nRow << " " << nNz << endl;
    // verifyCSR();
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
        
       
        // if(verPtr[i+1]-verPtr[i]!=sVerPtr[nrow]-sVerPtr[nrow-1]){
            // cout << "mistake" << verPtr[i+1]-verPtr[i] << " " << sVerPtr[nrow]-sVerPtr[nrow-1] << endl;
        // }
        // for(Size j=verPtr[i];j<verPtr[i+1]-1;j++)
        // {
            // if(verInd[j].id > verInd[j+1].id)
                // cout << " ordering mistake" << j << " " <<  verInd[j].id << " " << verInd[j+1].id << endl;
        // }
        
    }
    assert(count==nnz);
    assert(nrow==rowIdxs.size());
    
    subMtx.nRow=nrow;
    subMtx.nCol=nCol;
    subMtx.nNz=nnz;
    subMtx.maxDeg=max;
    subMtx.verPtr.swap(sVerPtr);
    subMtx.verInd.swap(sVerInd);
    // cout << "Verifying the output" << endl;
    // subMtx.verifyCSR();
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
    // cout << "Generating Similarity" << endl;
    vector<vector<Size> > graphCRSIdx(nRow);
    vector<vector<Val> > graphCRSVal(nRow);
    
    for(Size i=0; i<nRow; i++){
        for(Size j=i+1; j < nRow; j++){
            
            Val sum;
            getSimilarity(sum, i, j);
            graphCRSIdx[i].push_back(j);
            graphCRSVal[i].push_back(sum);
            graphCRSIdx[j].push_back(i);
            graphCRSVal[j].push_back(sum);
        }
    }
    // cout << "Finished similarity calculation" << endl;
    vector<Size> sVerPtr;
    ResizeVector<Size>(&sVerPtr,nRow+1);
    vector<Edge> sVerInd; 
    ResizeVector<Edge>(&sVerInd,nRow*(nRow-1));
    
    Size count=0;
    sVerPtr[0]=0;
    Size max=0,offset; 
    for(Size i=1;i<=nRow;i++){
        //cout << i << " ";
        offset=graphCRSIdx[i-1].size();
        sVerPtr[i]=sVerPtr[i-1]+offset;
        count=sVerPtr[i-1];
        // cout<<i-1<<" "<<verPtr[i-1]<<" "<<verPtr[i]<<": " << graphCRSIdx[i-1].size() << endl;
        for(Size j:SortIndexes(graphCRSIdx[i-1])){
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
    sMtx.nNz=count;
    sMtx.maxDeg=max;
    sMtx.verPtr.swap(sVerPtr);
    sMtx.verInd.swap(sVerInd);
    return true;
}


bool CSR::getSimilarity(Val& similarity, Size row1, Size row2) const{
    
    Val sum = 0;
    
    if(row2==-1){
        for(Size iIdx=verPtr[row1];iIdx<verPtr[row1+1];iIdx++){
            sum+= verInd[iIdx].weight*verInd[iIdx].weight;
        }
    }
    else if(row1==-1){
        for(Size iIdx=verPtr[row2];iIdx<verPtr[row2+1];iIdx++){
            sum+= verInd[iIdx].weight*verInd[iIdx].weight;
        }
    }
    else{
        Size jIdx = verPtr[row2];
        Size jEnd = verPtr[row2+1];
        
        for(Size iIdx=verPtr[row1];iIdx<verPtr[row1+1];iIdx++){
            
            while(jIdx<jEnd && verInd[jIdx].id< verInd[iIdx].id ){
                // cout << verInd[jIdx].weight*verInd[jIdx].weight << "+ ";
                sum+= verInd[jIdx].weight*verInd[jIdx].weight;
                jIdx++;
            }
            
            if(jIdx<jEnd && verInd[jIdx].id==verInd[iIdx].id){ 
                // cout  << (verInd[jIdx].weight-verInd[iIdx].weight)*(verInd[jIdx].weight-verInd[iIdx].weight) << "+ ";
                sum+= (verInd[jIdx].weight-verInd[iIdx].weight)*(verInd[jIdx].weight-verInd[iIdx].weight);
                jIdx++;
            }
            else {
                // cout << verInd[iIdx].weight*verInd[iIdx].weight << "+ ";
                sum+= verInd[iIdx].weight*verInd[iIdx].weight;
            }
        }
        
        
        while(jIdx<jEnd){
            // cout << verInd[jIdx].weight*verInd[jIdx].weight << "+ ";
            sum+= verInd[jIdx].weight*verInd[jIdx].weight;
            jIdx++;
        }
        // cout << endl;
    }
        
    //similarity = -pow(sum, 0.5);
    similarity= sum;
    // cout << "Similarity of " << row1 << " and " << row2 << " is " << similarity << endl;
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
    
    // cout << "reading file " << filename << endl;
    ifstream inf;
    inf.open(filename,ios::in|ios::binary);
    if(inf.is_open()){
        // cout << " ->opened ";
        inf.read((char*)&nRow, sizeof(Size));
        inf.read((char*)&nCol, sizeof(Size));
        inf.read((char*)&nNz, sizeof(Size));
        inf.read((char*)&maxDeg, sizeof(Size));
        
        ResizeVector<Size>(&verPtr, nRow+1);
        inf.read((char*)&verPtr[0], sizeof(Size) * (nRow+1));
        
        // cout << "-> read verPtr ";
        vector<Size> verId;
        ResizeVector<Size>(&verId, nNz);
        vector<Val> verWt;
        ResizeVector<Val>(&verWt, nNz);
        
        ResizeVector<Edge>(&verInd, nNz);
        
        inf.read((char*)&verId[0], sizeof(Size) * nNz);
        inf.read((char*)&verWt[0], sizeof(Val) * nNz);
        inf.close();
        // cout << "-> read verInd ";
        
        for( Size i=0; i<nNz ;i++){
            verInd[i].id = verId[i];
            verInd[i].weight = verWt[i];
        }
        // cout << "--> finished" << endl;
        return true;
    }
    // cout << endl;
    return false;
}


bool CSR::verifyCSR() const{
    bool passed=true;
    if(verPtr.size()!=nRow+1){
        cout << "Ver Ptr size mismatch"<< verPtr.size() << " " << nRow+1 << endl;
        passed=false;
    }
    if(verPtr[nRow]!=nNz){
        cout << "Ver Ptr end entry mismatch " << verPtr[nRow] << " " << nNz << endl;
        passed=false;
    }
    if(verPtr[0]!=0){
        cout << "Ver Ptr start entry mismatch " << verPtr[0] << endl;
        passed=false;
    }
    if(verInd.size()!=nNz){
        cout << "Ver Ind size mismatch " << verInd.size() << " " << nNz << endl;
        passed=false;
    }
     
    for(Size i=0; i<nRow; i++){
        if(verPtr[i]>verPtr[i+1]){
            cout << "Ver Ptr indexing mismatch "<< nRow << " " << nNz << endl;
            passed=false;
            for(Size j:verPtr){
                cout << j << " ";
            }cout << endl;
            return false;
        }
    }
    
    for(Size i=0; i<nRow; i++){
        for(Size j=verPtr[i]; j<verPtr[i+1]-1; j++){
            if(verInd[j].id>verInd[j+1].id){
                cout << "Ver Ind indexing mismatch " << nRow << " " << nNz << endl;
                passed=false;
                // for(Edge k:verInd){
                    // cout << k.id << " ";
                // }cout << endl;
                break;
            }
        }
    }
    
    
    return passed;
}
    
    

