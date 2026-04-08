#include "delimiters.hpp"
#include <iomanip>
#include <stdexcept>

namespace nspace
{
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        
        char c = '0';
        in >> c;
        if (in && (c != dest.exp))
        {
            in.setstate(std::ios::failbit);
            throw std::runtime_error(std::string("Expected delimiter '") + dest.exp + "', got '" + c + "'");
        }
        return in;
    }
    
    std::istream& operator>>(std::istream& in, StringIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        
        char quote;
        in >> quote;
        if (quote != '"')
        {
            in.setstate(std::ios::failbit);
            throw std::runtime_error("Expected opening quote '\"'");
        }
        
        std::getline(in, dest.ref, '"');
        return in;
    }
    
    std::istream& operator>>(std::istream& in, ULLHexIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        
        char c;
        in >> c;
        if (c != '0')
        {
            in.setstate(std::ios::failbit);
            throw std::runtime_error("Expected hex prefix '0'");
        }
        
        in >> c;
        if (c != 'x' && c != 'X')
        {
            in.setstate(std::ios::failbit);
            throw std::runtime_error("Expected hex prefix 'x' or 'X'");
        }
        
        in >> std::hex >> dest.ref;
        if (in.fail())
        {
            throw std::runtime_error("Failed to read hexadecimal number");
        }
        
        return in;
    }
    
    std::istream& operator>>(std::istream& in, ComplexIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        
        char hash, c;
        in >> hash >> c;
        if (hash != '#' || c != 'c')
        {
            in.setstate(std::ios::failbit);
            throw std::runtime_error("Expected '#c' prefix for complex number");
        }
        
        char openParen;
        in >> openParen;
        if (openParen != '(')
        {
            in.setstate(std::ios::failbit);
            throw std::runtime_error("Expected '(' after '#c'");
        }
        
        double real = 0.0, imag = 0.0;
        in >> real >> imag;
        if (in.fail())
        {
            throw std::runtime_error("Failed to read real and imaginary parts");
        }
        
        char closeParen;
        in >> closeParen;
        if (closeParen != ')')
        {
            in.setstate(std::ios::failbit);
            throw std::runtime_error("Expected ')' after complex parts");
        }
        
        if (in)
        {
            dest.ref = std::complex<double>(real, imag);
        }
        
        return in;
    }
}
