#pragma once
#include <vector>
#include <numeric>
#include <algorithm>
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


