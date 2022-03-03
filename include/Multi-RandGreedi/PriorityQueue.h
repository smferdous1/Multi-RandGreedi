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