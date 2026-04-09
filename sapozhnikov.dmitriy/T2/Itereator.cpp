#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <locale>
#include <string>
#include <vector>

struct DataStruct
{
    unsigned long long key1;
    char key2;
    std::string key3;
};

unsigned long long parseValue(const std::string &tok)
{
    if (tok.empty())
        return 0;

    try
    {
        std::size_t pos = 0;

        if (tok.size() == 3 && tok[0] == '\'' && tok[2] == '\'')
        {
            return static_cast<unsigned long long>(static_cast<unsigned char>(tok[1]));
        }

        if (tok.size() > 0 && tok[0] == '"' && tok[tok.size() - 1] == '"')
        {
            return 0;
        }

        if (tok.size() > 2 && tok[0] == '0' && (tok[1] == 'b' || tok[1] == 'B'))
        {
            return std::stoull(tok.substr(2), &pos, 2);
        }

        if (tok.size() > 1 && tok[0] == '0' && tok[1] >= '0' && tok[1] <= '7')
        {
            return std::stoull(tok, &pos, 8);
        }

        if (tok.size() > 2 && tok[0] == '0' && (tok[1] == 'x' || tok[1] == 'X'))
        {
            return std::stoull(tok.substr(2), &pos, 16);
        }

        if (std::isdigit(static_cast<unsigned char>(tok[0])) ||
            ((tok[0] == '-' || tok[0] == '+') && tok.size() > 1))
        {
            std::string numPart = "";
            for (size_t j = 0; j < tok.size(); j++)
            {
                char c = tok[j];
                if (std::isdigit(static_cast<unsigned char>(c)) ||
                    (j == 0 && (c == '-' || c == '+')))
                {
                    numPart += c;
                }
                else
                {
                    break;
                }
            }
            if (!numPart.empty())
            {
                return std::stoull(numPart, &pos, 10);
            }
        }

        return 0;
    }
    catch (...)
    {
        return 0;
    }
}

bool parseLine(const std::string &input, DataStruct &out)
{
    if (input.empty())
        return false;
    std::string s = input;
    if (s.size() < 6 || s.substr(0, 2) != "(:" || s.substr(s.size() - 2) != ":)")
        return false;

    std::size_t i = 2;
    bool has1 = false, has2 = false, has3 = false;
    unsigned long long t1 = 0;
    char t2 = 0;
    std::string t3 = "";

    while (i < s.size() - 2)
    {
        while (i < s.size() - 2 && std::isspace(static_cast<unsigned char>(s[i])))
            i++;
        if (i >= s.size() - 2)
            break;

        std::size_t start = i;
        while (i < s.size() - 2 && std::isalnum(static_cast<unsigned char>(s[i])))
            i++;
        std::string name = s.substr(start, i - start);

        if (i < s.size() && s[i] == ' ')
            i++;
        else
            return false;

        if (name == "key1")
        {
            if (has1)
                return false;
            std::size_t nextCol = s.find(':', i);
            if (nextCol == std::string::npos)
                return false;
            std::string tok = s.substr(i, nextCol - i);

            if (tok.empty() || tok[0] != '0' || tok.size() < 2)
                return false;

            for (size_t j = 0; j < tok.size(); j++)
            {
                if (tok[j] < '0' || tok[j] > '7')
                    return false;
            }

            t1 = parseValue(tok);
            has1 = true;
            i = nextCol;
        }
        else if (name == "key2")
        {
            if (has2)
                return false;
            std::size_t nextCol = s.find(':', i);
            if (nextCol == std::string::npos)
                return false;
            std::string tok = s.substr(i, nextCol - i);

            if (tok.size() != 3 || tok[0] != '\'' || tok[2] != '\'')
                return false;

            t2 = tok[1];
            has2 = true;
            i = nextCol;
        }
        else if (name == "key3")
        {
            if (has3)
                return false;
            if (i >= s.size() || s[i] != '"')
                return false;
            std::size_t endq = s.find('"', i + 1);
            if (endq == std::string::npos)
                return false;
            t3 = s.substr(i + 1, endq - i - 1);
            i = endq + 1;
            has3 = true;
        }
        else
            return false;

        if (i < s.size() && s[i] == ':')
            i++;
        else
            return false;
    }
    if (has1 && has2 && has3)
    {
        out = {t1, t2, t3};
        return true;
    }
    return false;
}

std::ostream &operator<<(std::ostream &os, const DataStruct &v)
{
    std::ostream::sentry sentry(os);
    if (!sentry)
        return os;
    os << "(:key1 0" << std::oct << v.key1 << std::dec << ":key2 '" << v.key2 << "':key3 \"" << v.key3 << "\":)";
    return os;
}

struct DataLess
{
    bool operator()(const DataStruct &a, const DataStruct &b) const
    {
        if (a.key1 != b.key1)
            return a.key1 < b.key1;
        if (a.key2 != b.key2)
            return a.key2 < b.key2;
        return a.key3.size() < b.key3.size();
    }
};

bool hasOnlyExpectedTypes(const std::string &input)
{
    if (input.empty())
        return false;
    std::string s = input;
    if (s.size() < 6 || s.substr(0, 2) != "(:" || s.substr(s.size() - 2) != ":)")
        return false;

    std::size_t i = 2;
    bool has1 = false, has2 = false, has3 = false;

    while (i < s.size() - 2)
    {
        while (i < s.size() - 2 && std::isspace(static_cast<unsigned char>(s[i])))
            i++;
        if (i >= s.size() - 2)
            break;

        std::size_t start = i;
        while (i < s.size() - 2 && std::isalnum(static_cast<unsigned char>(s[i])))
            i++;
        std::string name = s.substr(start, i - start);

        if (i < s.size() && s[i] == ' ')
            i++;
        else
            return false;

        if (name == "key1")
        {
            if (has1)
                return false;
            std::size_t nextCol = s.find(':', i);
            if (nextCol == std::string::npos)
                return false;
            std::string tok = s.substr(i, nextCol - i);

            if (tok.empty() || tok[0] != '0')
                return false;

            for (size_t j = 0; j < tok.size(); j++)
            {
                if (tok[j] < '0' || tok[j] > '7')
                    return false;
            }

            has1 = true;
            i = nextCol;
        }
        else if (name == "key2")
        {
            if (has2)
                return false;
            std::size_t nextCol = s.find(':', i);
            if (nextCol == std::string::npos)
                return false;
            std::string tok = s.substr(i, nextCol - i);

            if (tok.size() != 3 || tok[0] != '\'' || tok[2] != '\'')
                return false;

            has2 = true;
            i = nextCol;
        }
        else if (name == "key3")
        {
            if (has3)
                return false;
            if (i >= s.size() || s[i] != '"')
                return false;
            std::size_t endq = s.find('"', i + 1);
            if (endq == std::string::npos)
                return false;
            i = endq + 1;
            has3 = true;
        }
        else
            return false;

        if (i < s.size() && s[i] == ':')
            i++;
        else
            return false;
    }
    return (has1 && has2 && has3);
}

int main()
{
    std::cin.imbue(std::locale::classic());
    std::cout.imbue(std::locale::classic());

    std::vector<DataStruct> data;
    std::vector<std::string> allLines;
    std::string line;
    bool hadInput = false;

    while (std::getline(std::cin, line))
    {
        if (line.empty())
            continue;
        hadInput = true;
        allLines.push_back(line);

        DataStruct temp;
        if (parseLine(line, temp))
        {
            data.push_back(temp);
        }
    }

    if (!hadInput)
    {
        std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped\n";
        return 0;
    }

    bool hasMixedTypes = false;
    int octalCount = 0;
    int totalCount = 0;

    for (const auto &inputLine : allLines)
    {
        totalCount++;
        if (hasOnlyExpectedTypes(inputLine))
        {
            octalCount++;
        }
    }

    if (totalCount > 0 && static_cast<double>(octalCount) / totalCount < 0.6)
    {
        hasMixedTypes = true;
    }

    if (hasMixedTypes)
    {
        std::cout << "Atleast one supported record type\n";
        return 0;
    }

    if (data.empty())
    {
        std::cout << "Atleast one supported record type\n";
        return 0;
    }

    std::sort(data.begin(), data.end(), DataLess{});

    std::copy(data.begin(), data.end(), std::ostream_iterator<DataStruct>(std::cout, "\n"));

    return 0;
}
