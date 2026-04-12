
#ifndef OLDFORMAT_HPP
#define OLDFORMAT_HPP

#include <iostream>

class OldFormat
{
public:
    explicit OldFormat(std::basic_ios<char>& s);
    ~OldFormat();

    OldFormat(const OldFormat&) = delete;
    OldFormat& operator=(const OldFormat&) = delete;

private:
    std::basic_ios<char>& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios<char>::fmtflags fmt_;
};

#endif

