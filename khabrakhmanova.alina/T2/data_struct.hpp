#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <iostream>
#include <string>
#include <complex>
#include "io_types.hpp"
#include "iofmtguard.hpp"

struct DataStruct {
    unsigned long long key1;
    std::complex<double> key2;
    std::string key3;
};

std::istream& operator>>(std::istream& in, DataStruct& dest);
std::ostream& operator<<(std::ostream& out, const DataStruct& src);
bool parseFromString(const std::string& line, DataStruct& dest);
std::string formatULLLit(unsigned long long value);
std::string formatComplex(const std::complex<double>& value);

#endif
