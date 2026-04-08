#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <iostream>
#include <string>
#include <complex>

struct DataStruct
{
    unsigned long long key1;
    std::complex<double> key2;
    std::string key3;
};

unsigned long long parseBinary(const std::string& s);
std::string toBinaryString(unsigned long long n);
bool tryParseRecord(const std::string& record, DataStruct& ds);
bool comparator(const DataStruct& a, const DataStruct& b);

std::istream& operator>>(std::istream& in, DataStruct& ds);
std::ostream& operator<<(std::ostream& out, const DataStruct& ds);

#endif
