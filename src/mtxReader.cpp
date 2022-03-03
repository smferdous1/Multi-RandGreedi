#include "mtxReader.h"
#include <cstring>
using namespace std;

bool CSR::readMtx(char* filename)
{
    int count=0,i,j;
    int inp, m1, sym, edgecnt_;
    int numRow, numCol, nonZeros, numEdges;
    double f;
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
        
        vector<vector<int> > graphCRSIdx(nRow);
        vector<vector<double> > graphCRSVal(nRow);
        
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
        
        verPtr=new int[nRow+1];
        verInd=new Edge[nNz];

        verPtr[0]=0;
        int max=0,offset; 
        for(int i=1;i<=nRow;i++)
        {
            
            offset=graphCRSIdx[i-1].size();
            verPtr[i]=verPtr[i-1]+offset;
            count=verPtr[i-1];
            //cout<<i-1<<" "<<verPtr[i-1]<<" "<<verPtr[i]<<": ";
            for(int j=0;j<offset;j++)
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

        cout<<nRow<<" "<<nCol<<" "<<nNz<<endl;

    }
    else return false;

   
   return true;
}
