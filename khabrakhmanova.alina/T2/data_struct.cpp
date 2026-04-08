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

        while (isspace(in.peek())) in.get();

        if (in.peek() != '(')
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        char openParen;
        in >> openParen;

        char colon1;
        in >> colon1;
        if (colon1 != ':')
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        DataStruct input;
        bool key1Read = false;
        bool key2Read = false;
        bool key3Read = false;

        while (in && (!key1Read || !key2Read || !key3Read))
        {
            while (isspace(in.peek())) in.get();

            std::string label;
            in >> label;

            if (label != "key1" && label != "key2" && label != "key3")
            {
                in.setstate(std::ios::failbit);
                return in;
            }

            while (isspace(in.peek())) in.get();

            char colon;
            in >> colon;
            if (colon != ':')
            {
                in.setstate(std::ios::failbit);
                return in;
            }

            while (isspace(in.peek())) in.get();

            if (label == "key1" && !key1Read)
            {
                char c;
                in >> c;
                if (c != '0')
                {
                    in.setstate(std::ios::failbit);
                    return in;
                }

                in >> c;
                if (c != 'x' && c != 'X')
                {
                    in.setstate(std::ios::failbit);
                    return in;
                }

                in >> std::hex >> input.key1;
                if (in)
                    key1Read = true;
                else
                    return in;
            }
            else if (label == "key2" && !key2Read)
            {
                char hash, c;
                in >> hash >> c;
                if (hash != '#' || c != 'c')
                {
                    in.setstate(std::ios::failbit);
                    return in;
                }

                char openParen2;
                in >> openParen2;
                if (openParen2 != '(')
                {
                    in.setstate(std::ios::failbit);
                    return in;
                }

                double real, imag;
                in >> real >> imag;

                char closeParen2;
                in >> closeParen2;
                if (closeParen2 != ')')
                {
                    in.setstate(std::ios::failbit);
                    return in;
                }

                input.key2 = std::complex<double>(real, imag);
                key2Read = true;
            }
            else if (label == "key3" && !key3Read)
            {
                char quote;
                in >> quote;
                if (quote != '"')
                {
                    in.setstate(std::ios::failbit);
                    return in;
                }
                std::getline(in, input.key3, '"');
                key3Read = true;
            }
            else
            {
                in.setstate(std::ios::failbit);
                return in;
            }

            while (isspace(in.peek())) in.get();

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
                in.setstate(std::ios::failbit);
                return in;
            }
        }

        if (key1Read && key2Read && key3Read)
        {
            dest = input;
        }
        else
        {
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
