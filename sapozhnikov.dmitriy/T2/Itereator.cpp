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
   std::size_t i = 0;
   std::string s = input;

   if (s.size() < 6 || s[i++] != '(' || s[i++] != ':') return false;

   bool has1 = false, has2 = false, has3 = false;

   while (i < s.size()) {
      while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i]))) i++;

      if (s.compare(i, 2, ":)") == 0) {
        i += 2;
        break;
      }

      std::size_t start = i;
      while (i < s.size() && std::isalnum(static_cast<unsigned char>(s[i]))) i++;
      std::string name = s.substr(start, i - start);

      if (i < s.size() && s[i] == ' ') i++;

      if (name == "key1"){
        if (has1) return false;
        std::size_t nextCol = s.find(':', i);
        if (nextCol == std::string::npos) return false;
        
        std::string tok = s.substr(i, nextCol - i);
        try{
           out.key1 = std::stoull(tok, nullptr, 0);
           has1 = true;
        } catch (...) { return false; }
        i = nextCol;
      }
      else if (name == "key2"){
        if (has2) return false;
        std::size_t open = s.find('\'', i);
        if (open == std::string::npos || open + 2 >= s.size() || s[open + 2] != '\'') return false;
        out.key2 = s[open + 1];
        i = open + 3;
        has2 = true;
      }
      else if (name == "key3"){
        if (has3) return false;
        std::size_t open = s.find('"', i);
        if (open == std::string::npos) return false;
        std::size_t close = s.find('"', open + 1);
        if (close == std::string::npos) return false;
        
        out.key3 = s.substr(open + 1, close - open - 1);
        i = close + 1;
        has3 = true;
      }

      if (i < s.size() && s[i] == ':'){
        if (i + 1 < s.size() && s[i+1] == ')'){
           i += 2;
           break;
        }
        i++;
      }
   }

   return has1 && has2 && has3;
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
    if (hadInput)
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

  for (const auto& d : data)
  {
    std::cout << d << "\n";
  }

  return 0;
}
