
#include "DataStruct.hpp"
#include <string>
#include <sstream>
#include <cctype>

namespace nspace
{
    std::istream& operator>>(std::istream& in, DataStruct& dest)
    {
        std::string line;
        if (!std::getline(in, line))
        {
            return in;
        }

        size_t key1_pos = line.find(":key1 ");
        if (key1_pos == std::string::npos)
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        size_t key1_start = key1_pos + 6;
        size_t key1_end = line.find("LL", key1_start);
        if (key1_end == std::string::npos)
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        std::string key1_str = line.substr(key1_start, key1_end - key1_start);
        bool key1_ok = true;
        for (size_t i = 0; i < key1_str.size(); ++i)
        {
            if (i == 0 && key1_str[i] == '-') continue;
            if (!std::isdigit(static_cast<unsigned char>(key1_str[i])))
            {
                key1_ok = false;
                break;
            }
        }
        if (!key1_ok || key1_str.empty())
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        dest.key1 = std::stoll(key1_str);

        size_t key2_pos = line.find(":key2 (:N ");
        if (key2_pos == std::string::npos)
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        size_t num_start = key2_pos + 11;
        size_t d_pos = line.find(":D ", num_start);
        if (d_pos == std::string::npos)
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        std::string num_str = line.substr(num_start, d_pos - num_start);
        bool num_ok = true;
        for (size_t i = 0; i < num_str.size(); ++i)
        {
            if (i == 0 && num_str[i] == '-') continue;
            if (!std::isdigit(static_cast<unsigned char>(num_str[i])))
            {
                num_ok = false;
                break;
            }
        }
        if (!num_ok || num_str.empty())
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        long long numerator = std::stoll(num_str);

        size_t den_start = d_pos + 3;
        size_t close_pos = line.find(":)", den_start);
        if (close_pos == std::string::npos)
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        std::string den_str = line.substr(den_start, close_pos - den_start);
        bool den_ok = true;
        for (size_t i = 0; i < den_str.size(); ++i)
        {
            if (!std::isdigit(static_cast<unsigned char>(den_str[i])))
            {
                den_ok = false;
                break;
            }
        }
        if (!den_ok || den_str.empty())
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        unsigned long long denominator = std::stoull(den_str);

        if (denominator == 0)
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        dest.key2 = { numerator, denominator };

        size_t key3_pos = line.find(":key3 \"");
        if (key3_pos == std::string::npos)
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        size_t key3_start = key3_pos + 7;
        size_t quote_pos = line.find("\"", key3_start);
        if (quote_pos == std::string::npos)
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        dest.key3 = line.substr(key3_start, quote_pos - key3_start);

        if (line.find(":)", quote_pos) == std::string::npos)
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& src)
    {
        out << "(:key1 " << src.key1 << "LL"
            << ":key2 (:N " << src.key2.first << ":D " << src.key2.second << ":)"
            << ":key3 \"" << src.key3 << "\":)";
        return out;
    }
}



