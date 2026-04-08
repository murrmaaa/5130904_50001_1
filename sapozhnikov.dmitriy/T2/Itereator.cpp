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



bool parseLine(const std::string& input, DataStruct& out)
{
  std::string s = input;

  std::size_t i = 0;


  if (s.size() < 4) return false;
  if (s[i++] != '(') return false;
  if (s[i++] != ':') return false;

  bool has1 = false, has2 = false, has3 = false;

  while (true)
  {
    std::size_t sp = s.find(' ', i);
    if (sp == std::string::npos) return false;
    std::string name = s.substr(i, sp - i);

    i = sp;
    if (i >= s.size() || s[i] != ' ') return false;
    ++i;

    if (name == "key1")
    {
    if (has1) return false;


    std::size_t col = s.find(':', i);
    if (col == std::string::npos) return false;
    std::string tok = s.substr(i, col - i);


    if (tok.empty() || tok[0] != '0') return false;
    for (char c : tok) if (c < '0' || c > '7') return false;


    try
    {
      std::size_t pos = 0;
      unsigned long long v = std::stoull(tok, &pos, 8);
      if (pos != tok.size()) return false;
      out.key1 = v;
    }
    catch (...)
    {
      return false;
    }

    i = col;
    has1 = true;
    }
    else if (name == "key2")
    {
    if (has2) return false;


    if (i + 2 >= s.size()) return false;
    if (s[i] != '\'') return false;
    out.key2 = s[i + 1];
    if (s[i + 2] != '\'') return false;
    i += 3;

    has2 = true;
    }
    else if (name == "key3")
    {
    if (has3) return false;


    if (i >= s.size() || s[i] != '"') return false;
    ++i;
    std::size_t endq = s.find('"', i);
    if (endq == std::string::npos) return false;

    out.key3 = s.substr(i, endq - i);
    i = endq + 1;

    has3 = true;
    }
    else
    {
    return false;
    }


    if (i >= s.size() || s[i] != ':') return false;
    ++i;


    if (i < s.size() && s[i] == ')')
    {
    ++i;
    break;
    }

  }


  return (i == s.size()) && has1 && has2 && has3;
}


std::istream& operator>>(std::istream& in, DataStruct& v)
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


std::ostream& operator<<(std::ostream& os, const DataStruct& v)
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
  bool operator()(const DataStruct& a, const DataStruct& b) const
  {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;
    return a.key3.size() < b.key3.size();
  }
};

int main()
{
  std::cin.imbue(std::locale::classic());
  std::cout.imbue(std::locale::classic());

  std::vector<DataStruct> data;

  std::copy(std::istream_iterator<DataStruct>(std::cin),
    std::istream_iterator<DataStruct>(),
    std::back_inserter(data));

  std::sort(data.begin(), data.end(), DataLess{});

  std::copy(data.begin(), data.end(),
    std::ostream_iterator<DataStruct>(std::cout, "\n"));
}
