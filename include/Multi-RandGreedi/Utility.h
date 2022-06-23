#pragma once
#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <fstream>
#include<sys/time.h>

#include "Types.h"

template<class T>
inline void ReserveVector(std::vector<T>* vec, Size size) {
  std::vector<T> tmpVec;
  tmpVec.reserve(size);
  vec->swap(tmpVec);
}

template<class T>
inline void ResizeVector(std::vector<T>* vec, Size size) {
  std::vector<T> tmpVec;
  tmpVec.reserve(size);
  tmpVec.resize(size);
  vec->swap(tmpVec);
}

template <typename T>
std::vector<Size> SortIndexes(const std::vector<T> &v) {

  // initialize original index locations
  std::vector<Size> idx(v.size());
  std::iota(idx.begin(), idx.end(), 0);
 
  std::stable_sort(idx.begin(), idx.end(),
       [&v](Size i1, Size i2) {return v[i1] < v[i2];});

  return idx;
}


inline void CurrentTime(Val &nowTime){
    struct timeval tz;
    struct timezone tx;
    gettimeofday(&tz,&tx);
    nowTime = (Val) tz.tv_sec+(Val) tz.tv_usec/1000000.0;
}

template <typename T>
inline void WriteArray(char* filename, const std::vector<T> &v){
    
    std::ofstream of;
    of.open(filename,std::ios::out|std::ios::binary);
    Size s = v.size();
    if(of.is_open()){
        of.write((char*)&s, sizeof(Size));
        of.write((char*)&v[0], sizeof(T)*s);
        of.close();
    }
    else{
       std::cout << "unable to open " << filename << std::endl; 
    }
}

template <typename T>
inline void ReadArray(char* filename, std::vector<T> &v){
    
    std::ifstream inf;
    inf.open(filename,std::ios::in|std::ios::binary);
    Size s;
    if(inf.is_open()){
        inf.read((char*)&s, sizeof(Size));
        ResizeVector<T>(&v, s);
        inf.read((char*)&v[0], sizeof(T)*s);
        inf.close();
    }
    else{
       std::cout << "unable to open " << filename << std::endl; 
    }
}

