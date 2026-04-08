#ifndef DATASORT_H
#define DATASORT_H

#include "data_struct.hpp"
#include <cmath>

inline bool compareDataStruct(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) {
        return a.key1 < b.key1;
    }
    double modA = std::abs(a.key2);
    double modB = std::abs(b.key2);
    if (std::abs(modA - modB) > 1e-10) {
        return modA < modB;
    }
    return a.key3.length() < b.key3.length();
}

#endif
