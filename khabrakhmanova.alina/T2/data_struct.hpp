#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <iostream>
#include <string>
#include <complex>

namespace nspace
{
    struct DataStruct
    {
        unsigned long long key1;           // ULL HEX
        std::complex<double> key2;         // CMP LSP
        std::string key3;
    };
    
    bool compareData(const DataStruct& a, const DataStruct& b);
    std::istream& operator>>(std::istream& in, DataStruct& dest);
    std::ostream& operator<<(std::ostream& out, const DataStruct& src);
}

#endif
