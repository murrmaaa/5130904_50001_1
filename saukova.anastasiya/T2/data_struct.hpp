#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <iostream>
#include <string>

namespace anastasiasaukova
{
  struct DataStruct
  {
    double key1_;
    long long key2_;
    std::string key3_;
  };

  struct DelimiterIO { char expected; };
  struct DoubleLitIO { double &ref; };
  struct LongLongLitIO { long long &ref; };
  struct StringIO { std::string &ref; };

  class iofmtguard
  {
  public:
    explicit iofmtguard(std::basic_ios< char > &s);
    ~iofmtguard();
  private:
    std::basic_ios< char > &s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
  };

  std::istream &operator>>(std::istream &in, DelimiterIO &&dest);
  std::istream &operator>>(std::istream &in, DoubleLitIO &&dest);
  std::istream &operator>>(std::istream &in, LongLongLitIO &&dest);
  std::istream &operator>>(std::istream &in, StringIO &&dest);
  std::istream &operator>>(std::istream &in, DataStruct &dest);
  std::ostream &operator<<(std::ostream &out, const DataStruct &src);

  bool compareDataStruct(const DataStruct &lhs, const DataStruct &rhs);
}
#endif
