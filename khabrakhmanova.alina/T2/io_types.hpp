#ifndef IO_TYPES_HPP
#define IO_TYPES_HPP

#include <iostream>
#include <string>
#include <complex>

struct DelimiterIO {
    char exp;
};

struct ULLLitIO {
    unsigned long long& ref;
};

struct ComplexIO {
    std::complex<double>& ref;
};

struct StringIO {
    std::string& ref;
};

struct LabelIO {
    std::string exp;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
std::istream& operator>>(std::istream& in, ULLLitIO&& dest);
std::istream& operator>>(std::istream& in, ComplexIO&& dest);
std::istream& operator>>(std::istream& in, StringIO&& dest);
std::istream& operator>>(std::istream& in, LabelIO&& dest);

#endif
