#ifndef DATA_IO_H
#define DATA_IO_H

#include <iostream>
#include <string>
#include "dataStruct.hpp"

using Ull = unsigned long long;

struct DelimiterIO {
    char expected;
};

struct DoubleIO {
    double& ref;
};

struct StringIO {
    std::string& ref;
};

struct UllIO {
    Ull& ref;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
std::istream& operator>>(std::istream& in, DoubleIO&& dest);
std::istream& operator>>(std::istream& in, StringIO&& dest);
std::istream& operator>>(std::istream& in, UllIO&& dest);
std::istream& operator>>(std::istream& in, DataStruct& dest);
std::ostream& operator<<(std::ostream& out, const DataStruct& dest);

#endif
