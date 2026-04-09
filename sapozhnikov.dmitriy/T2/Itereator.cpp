#include <algorithm>
#include <cctype>
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

bool parseLine(const std::string &input, DataStruct &out)
{
  std::string s = input;
  if (s.size() < 4 || s.front() != '(' || s.back() != ')')
    return false;

  std::size_t i = 0;
  if (s[i++] != '(' || s[i++] != ':')
    return false;

  bool has1 = false, has2 = false, has3 = false;

  while (i < s.size())
  {
    std::size_t sp = s.find(' ', i);
    if (sp == std::string::npos)
      break;
    std::string name = s.substr(i, sp - i);
    i = sp + 1;

    if (name == "key1")
    {
      if (has1)
        return false;
      std::size_t col = s.find(':', i);
      if (col == std::string::npos)
        return false;
      std::string tok = s.substr(i, col - i);

      try
      {
        std::size_t pos = 0;
        unsigned long long v = std::stoull(tok, &pos, 8);
        if (pos != tok.size())
          return false;
        out.key1 = v;
        has1 = true;
      }
      catch (...)
      {
        return false;
      }
      i = col + 1;
    }
    else if (name == "key2")
    {
      if (has2)
        return false;
      if (i + 2 >= s.size() || s[i] != '\'' || s[i + 2] != '\'')
        return false;
      out.key2 = s[i + 1];
      i += 3;
      if (i < s.size() && s[i] == ':')
        i++;
      else
        return false;
      has2 = true;
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
      out.key3 = s.substr(i + 1, endq - i - 1);
      i = endq + 1;
      if (i < s.size() && s[i] == ':')
        i++;
      else
        return false;
      has3 = true;
    }
    else
      return false;

    if (i < s.size() && s[i] == ')')
    {
      i++;
      break;
    }
  }

  return (has1 && has2 && has3);
}

std::istream &operator>>(std::istream &in, DataStruct &v)
{
  std::string line;
  while (std::getline(in, line))
  {
    DataStruct tmp{};
    if (parseLine(line, tmp))
    {
      v = tmp;
      return in;
    }
  }

  in.setstate(std::ios::failbit);
  return in;
}

std::ostream &operator<<(std::ostream &os, const DataStruct &v)
{
  os << "(:key1 ";

  std::ios::fmtflags f = os.flags();
  os << std::showbase << std::oct << v.key1;
  os.flags(f);

  os << ":key2 '" << v.key2 << "':key3 \"" << v.key3 << "\":)";
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

int main()
{
  std::cin.imbue(std::locale::classic());
  std::cout.imbue(std::locale::classic());

  std::vector<DataStruct> data;
  bool hadInput = false;

  std::string line;
  while (std::getline(std::cin, line))
  {
    hadInput = true;
    DataStruct tmp{};
    if (parseLine(line, tmp))
    {
      data.push_back(tmp);
    }
  }

  if (data.empty())
  {
    if (std::cin.eof())
    {
      std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped\n";
    }
    else
    {
      std::cout << "Atleast one supported record type\n";
    }
    return 0;
  }

  std::sort(data.begin(), data.end(), DataLess{});

  for (const auto &d : data)
  {
    std::cout << d << "\n";
  }

  return 0;
}
