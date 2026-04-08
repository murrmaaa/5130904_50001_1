#include "data_struct.hpp"
#include "delimiters.hpp"
#include "iofmtguard.hpp"
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <stdexcept>
#include <sstream>

namespace nspace
{
    bool compareData(const DataStruct& a, const DataStruct& b)
    {
        if (a.key1 != b.key1)
        {
            return a.key1 < b.key1;
        }
        
        double modA = std::abs(a.key2);
        double modB = std::abs(b.key2);
        
        const double epsilon = 1e-10;
        if (std::abs(modA - modB) > epsilon)
        {
            return modA < modB;
        }
        
        return a.key3.length() < b.key3.length();
    }
    
    std::istream& operator>>(std::istream& in, DataStruct& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        
        std::streampos pos = in.tellg();
        
        DataStruct input;
        
        try
        {
            char openParen, colon1;
            in >> openParen >> colon1;
            if (openParen != '(' || colon1 != ':')
            {
                throw std::runtime_error("Expected '(:' at start of record");
            }
            
            bool key1Read = false;
            bool key2Read = false;
            bool key3Read = false;
            
            while (in && (!key1Read || !key2Read || !key3Read))
            {
                std::string label;
                in >> label;
                
                if (label != "key1" && label != "key2" && label != "key3")
                {
                    throw std::runtime_error("Invalid label: " + label);
                }
                
                char colon;
                in >> colon;
                if (colon != ':')
                {
                    throw std::runtime_error("Expected ':' after label");
                }
                
                if (label == "key1" && !key1Read)
                {
                    in >> ULLHexIO{input.key1};
                    if (in)
                        key1Read = true;
                    else
                        throw std::runtime_error("Failed to read key1 as ULL HEX");
                }
                else if (label == "key2" && !key2Read)
                {
                    in >> ComplexIO{input.key2};
                    if (in)
                        key2Read = true;
                    else
                        throw std::runtime_error("Failed to read key2 as complex");
                }
                else if (label == "key3" && !key3Read)
                {
                    in >> StringIO{input.key3};
                    if (in)
                        key3Read = true;
                    else
                        throw std::runtime_error("Failed to read key3");
                }
                else
                {
                    throw std::runtime_error("Duplicate label: " + label);
                }
                
                char next;
                in >> next;
                if (next == ':')
                {
                    continue;
                }
                else if (next == ')')
                {
                    break;
                }
                else
                {
                    throw std::runtime_error("Expected ':' or ')' after value");
                }
            }
            
            if (!key1Read || !key2Read || !key3Read)
            {
                throw std::runtime_error("Missing required fields");
            }
            
            dest = std::move(input);
        }
        catch (const std::exception&)
        {
            in.clear();
            in.seekg(pos);
            in.setstate(std::ios::failbit);
        }
        
        return in;
    }
    
    std::ostream& operator<<(std::ostream& out, const DataStruct& src)
    {
        std::ostream::sentry sentry(out);
        if (!sentry) return out;
        
        iofmtguard fmtguard(out);
        
        out << "(:";
        out << "key1 0x" << std::hex << std::uppercase << src.key1 << ":";
        out << "key2 #c(" << std::fixed << std::setprecision(1)
            << src.key2.real() << " " << src.key2.imag() << "):";
        out << "key3 \"" << src.key3 << "\":";
        out << ")";
        
        return out;
    }
}
