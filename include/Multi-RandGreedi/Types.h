#pragma once
#include <limits>

typedef std::size_t Size;
typedef double Val;

struct EdgeE {
    Size head;
    Size id;         // Edge tail
    float weight;  // Edge weight
};
struct Edge {
    Size id;         // Edge tail
    Val weight;  // Edge weight
};

//constexpr Val negInfVal = std::numeric_limits<double>::lowest();

constexpr Val negInfVal = -2000;