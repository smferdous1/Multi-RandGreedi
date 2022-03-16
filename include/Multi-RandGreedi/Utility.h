#pragma once
#include <vector>

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
vector<Size> SortIndexes(const vector<T> &v) {

  // initialize original index locations
  vector<Size> idx(v.size());
  iota(idx.begin(), idx.end(), 0);
 
  stable_sort(idx.begin(), idx.end(),
       [&v](Size i1, Size i2) {return v[i1] < v[i2];});

  return idx;
}


