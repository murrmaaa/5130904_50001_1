#include "DataStruct.h"
#include <iomanip>
#include <sstream>
#include <cctype>
#include <vector>

namespace nspace
{
    // Парсинг восьмеричного числа (формат: 076, 01001)
    bool parseOct(const std::string& value, unsigned long long& result)
    {
        if (value.empty() || value[0] != '0')
            return false;
        for (char c : value)
        {
            if (c < '0' || c > '7')
                return false;
        }
        result = std::stoull(value, nullptr, 8);
        return true;
    }

    // Парсинг комплексного числа (формат: #c(1.0 -1.0))
    bool parseComplex(const std::string& value, std::complex<double>& result)
    {
        if (value.size() < 5 || value.substr(0, 3) != "#c(" || value.back() != ')')
            return false;
        std::string inner = value.substr(3, value.size() - 4);
        std::istringstream iss(inner);
        double real, imag;
        if (!(iss >> real >> imag))
            return false;
        result = std::complex<double>(real, imag);
        return true;
    }

    // Парсинг строки в кавычках
    bool parseString(const std::string& value, std::string& result)
    {
        if (value.size() < 2 || value[0] != '"' || value.back() != '"')
            return false;
        result = value.substr(1, value.size() - 2);
        return true;
    }

    // Парсинг одной записи
    bool parseLine(const std::string& line, DataStruct& dest)
    {
        if (line.empty() || line[0] != '(' || line.back() != ')')
            return false;

        std::string content = line.substr(1, line.size() - 2);

        // Разбиваем на токены по символу ':' (но не внутри кавычек)
        std::vector<std::string> tokens;
        std::string current;
        bool inQuotes = false;

        for (char c : content)
        {
            if (c == '"')
            {
                inQuotes = !inQuotes;
                current += c;
                continue;
            }
            if (!inQuotes && c == ':')
            {
                if (!current.empty())
                {
                    tokens.push_back(current);
                    current.clear();
                }
                continue;
            }
            current += c;
        }
        if (!current.empty())
            tokens.push_back(current);

        DataStruct temp;
        bool hasKey1 = false, hasKey2 = false, hasKey3 = false;

        for (const auto& token : tokens)
        {
            size_t spacePos = token.find_first_of(" \t");
            if (spacePos == std::string::npos)
                continue;

            std::string key = token.substr(0, spacePos);
            std::string value = token.substr(spacePos + 1);

            if (key == "key1")
            {
                if (parseOct(value, temp.key1))
                    hasKey1 = true;
            }
            else if (key == "key2")
            {
                if (parseComplex(value, temp.key2))
                    hasKey2 = true;
            }
            else if (key == "key3")
            {
                if (parseString(value, temp.key3))
                    hasKey3 = true;
            }
        }

        if (hasKey1 && hasKey2 && hasKey3)
        {
            dest = temp;
            return true;
        }
        return false;
    }

    // Оператор чтения
    std::istream& operator>>(std::istream& in, DataStruct& dest)
    {
        std::string line;
        while (std::getline(in, line))
        {
            if (line.empty())
                continue;

            DataStruct temp;
            if (parseLine(line, temp))
            {
                dest = temp;
                return in;
            }
        }
        in.setstate(std::ios::failbit);
        return in;
    }

    // Оператор вывода
    std::ostream& operator<<(std::ostream& out, const DataStruct& src)
    {
        out << "(:key1 0" << std::oct << src.key1
            << ":key2 #c(" << std::fixed << std::setprecision(1)
            << src.key2.real() << " " << src.key2.imag() << "):key3 \""
            << src.key3 << "\":)";
        return out;
    }
}
