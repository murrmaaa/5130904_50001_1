#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <iostream>
#include <string>
#include <complex>

namespace nspace
{
    struct DataStruct
    {
        unsigned long long key1;
        std::complex<double> key2;
        std::string key3;
    };

    // Вспомогательные структуры
    struct DelimiterIO { char exp; }; // для разделителей
    struct OctIO { unsigned long long& ref; }; // для восьмиричного числа
    struct ComplexIO { std::complex<double>& ref; }; // для комплексного числа
    struct StringIO { std::string& ref; }; // для строки
    struct LabelIO { std::string exp; }; // для проверки структуры записи

    class Iofmtguard
    {
    private:
        std::basic_ios<char>& s_;
        std::streamsize width_;
        char fill_;
        std::streamsize precision_;
        std::basic_ios<char>::fmtflags fmt_;
    public:
        explicit Iofmtguard(std::basic_ios<char>& s);
        ~Iofmtguard();
    };

    // Операторы для вспомогательных структур
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
    std::istream& operator>>(std::istream& in, OctIO&& dest);
    std::istream& operator>>(std::istream& in, ComplexIO&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);
    std::istream& operator>>(std::istream& in, LabelIO&& dest);

    // Оператор для основной структуры
    std::istream& operator>>(std::istream& in, DataStruct& dest);
    std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
}

#endif
