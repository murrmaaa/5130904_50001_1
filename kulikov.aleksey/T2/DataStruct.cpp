#include "DataStruct.hpp"

#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <cmath>

static unsigned long long parseBinary(const std::string& s)
{
    if (s.size() < 3)
        throw std::invalid_argument("Binary literal too short");

    if (s[0] != '0' || (s[1] != 'b' && s[1] != 'B'))
        throw std::invalid_argument("Not a binary literal");

    for (size_t i = 2; i < s.size(); i++)
    {
        if (s[i] != '0' && s[i] != '1')
            throw std::invalid_argument("Invalid binary digit");
    }

    unsigned long long result = 0;
    for (size_t i = 2; i < s.size(); i++)
    {
        result = result * 2 + static_cast<unsigned long long>(s[i] - '0');
    }

    return result;
}


static bool tryParseRecord(const std::string& record, DataStruct& ds)
{
    std::istringstream ss(record);
    char c;

    if (!(ss >> c) || c != '(')
        return false;

    if (!ss.get(c) || c != ':')
        return false;

    bool hasKey1 = false;
    bool hasKey2 = false;
    bool hasKey3 = false;
    DataStruct temp{};

    while (ss)
    {
        if (!ss.get(c))
            return false;

        if (c == ')')
            break;

        std::string key;
        key += c;
        while (ss.get(c) && c != ' ')
            key += c;

        if (!ss || c != ' ')
            return false;

        if (key == "key1")
        {
            if (hasKey1)
                return false;

            std::string token;
            while (ss.get(c) && c != ':')
                token += c;

            if (!ss)
                return false;

            try
            {
                temp.key1 = parseBinary(token);
                temp.key1str = token;
                hasKey1 = true;
            }
            catch (const std::exception&)
            {
                return false;
            }
        }
        else if (key == "key2")
        {
            if (hasKey2)
                return false;

            char h, ch, op;
            if (!ss.get(h) || h != '#')
                return false;
            if (!ss.get(ch) || ch != 'c')
                return false;
            if (!ss.get(op) || op != '(')
                return false;

            double real, imag;
            char cp;
            if (!(ss >> real >> imag >> cp) || cp != ')')
                return false;

            if (!ss.get(c) || c != ':')
                return false;

            temp.key2 = std::complex<double>(real, imag);
            hasKey2 = true;
        }
        else if (key == "key3")
        {
            if (hasKey3)
                return false;

            char q;
            if (!ss.get(q) || q != '"')
                return false;

            std::string str;
            while (ss.get(c) && c != '"')
                str += c;

            if (!ss)
                return false;

            if (!ss.get(c) || c != ':')
                return false;

            temp.key3 = str;
            hasKey3 = true;
        }
        else
        {
            return false;
        }
    }

    if (!hasKey1 || !hasKey2 || !hasKey3)
        return false;

    ds = temp;
    return true;
}

std::istream& operator>>(std::istream& in, DataStruct& ds)
{
    while (in)
    {
        char c;
        while (in.get(c) && c != '(');

        if (!in)
            break;

        std::string record = "(";
        bool inString = false;
        int depth = 1;

        while (in.get(c) && depth > 0)
        {
            record += c;

            if (c == '"')
                inString = !inString;

            if (!inString)
            {
                if (c == '(')
                    depth++;
                else if (c == ')')
                    depth--;
            }
        }

        if (depth != 0)
            break;

        if (tryParseRecord(record, ds))
            return in;
    }

    in.setstate(std::ios::failbit);
    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& ds)
{
    out << "(:key1 " << ds.key1str;
    out << ":key2 #c(";
    out << std::fixed << std::setprecision(1);
    out << ds.key2.real() << " " << ds.key2.imag() << ")";
    out << ":key3 \"" << ds.key3 << "\":)";
    return out;
}

bool comparator(const DataStruct& a, const DataStruct& b)
{
    if (a.key1 != b.key1)
        return a.key1 < b.key1;

    double absA = std::abs(a.key2);
    double absB = std::abs(b.key2);
    if (absA != absB)
        return absA < absB;

    return a.key3.size() < b.key3.size();
}
