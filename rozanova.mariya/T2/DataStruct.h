#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <iostream>
#include <string>
#include <complex>
#include <iomanip>

namespace nspace
{
    struct DataStruct
    {
        unsigned long long key1;
        std::complex<double> key2;
        std::string key3;
    };

    // Вспомогательные структуры для ввода
    struct DelimiterIO 
    { 
        char exp; 
    };
    
    struct OctIO 
    { 
        unsigned long long& ref; 
    };
    
    struct ComplexIO 
    { 
        std::complex<double>& ref; 
    };
    
    struct StringIO 
    { 
        std::string& ref; 
    };
    
    struct LabelIO 
    { 
        std::string exp; 
    };

    // Класс для сохранения/восстановления форматирования потока
    class Iofmtguard
    {
    public:
        explicit Iofmtguard(std::basic_ios<char>& s);
        ~Iofmtguard();
        
        Iofmtguard(const Iofmtguard&) = delete;
        Iofmtguard& operator=(const Iofmtguard&) = delete;

    private:
        std::basic_ios<char>& s_;
        std::streamsize width_;
        char fill_;
        std::streamsize precision_;
        std::basic_ios<char>::fmtflags fmt_;
    };

    // Операторы ввода для вспомогательных структур
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
    std::istream& operator>>(std::istream& in, OctIO&& dest);
    std::istream& operator>>(std::istream& in, ComplexIO&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);
    std::istream& operator>>(std::istream& in, LabelIO&& dest);

    // Операторы ввода/вывода для основной структуры
    std::istream& operator>>(std::istream& in, DataStruct& dest);
    std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
}

#endif
