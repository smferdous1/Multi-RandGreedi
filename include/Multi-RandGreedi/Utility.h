#ifndef UTILITY_H
#define UTILITY_H

#include <vector>

template<class T>
inline void ReserveVector(std::vector<T>* vec, int size) {
  std::vector<T> tmpVec;
  tmpVec.reserve(size);
  vec->swap(tmpVec);
}

template<class T>
inline void ResizeVector(std::vector<T>* vec, int size) {
  std::vector<T> tmpVec;
  tmpVec.reserve(size);
  tmpVec.resize(size);
  vec->swap(tmpVec);
}

template<class K, class V>
class KeyValue {
    public:
    K key;
    V value;
    
    KeyValue(){}
    ~KeyValue(){}
}

bool CompareKV( KeyValue a, KeyValue b){
    return a.value<b.value;
}

template<class K, class V>
class PriorityQ_KV {
    public:
    K key;
    V value;
    
    KeyValue(){}
    ~KeyValue(){}
}

#endif // UTILITY_H
