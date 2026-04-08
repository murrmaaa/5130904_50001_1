#ifndef DELIMITERS_HPP
#define DELIMITERS_HPP

#include <iostream>
#include <string>
#include <complex>

namespace nspace
{
    struct DelimiterIO
    {
        char exp;
    };
    
    struct StringIO
    {
        std::string& ref;
    };
    
    struct ULLHexIO
    {
        unsigned long long& ref;
    };
    
    struct ComplexIO
    {
        std::complex<double>& ref;
    };
    
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);
    std::istream& operator>>(std::istream& in, ULLHexIO&& dest);
    std::istream& operator>>(std::istream& in, ComplexIO&& dest);
}

#endif
