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


