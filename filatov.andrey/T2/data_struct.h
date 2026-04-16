#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <complex>
#include <string>
#include <iosfwd>

struct DataStruct {
    long long key1;                    // SLL LIT
    std::complex<double> key2;         // CMP LSP
    std::string key3;
};

std::istream& operator>>(std::istream& is, DataStruct& ds);
std::ostream& operator<<(std::ostream& os, const DataStruct& ds);

#endif // DATA_STRUCT_H
