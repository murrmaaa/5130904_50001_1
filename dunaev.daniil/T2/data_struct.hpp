#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <iosfwd>
#include <string>

struct DataStruct
{
  double key1;
  unsigned long long key2;
  std::string key3;
  std::string key2Digits;
};

bool compareDataStruct(const DataStruct& lhs, const DataStruct& rhs);

std::istream& operator>>(std::istream& in, DataStruct& value);
std::ostream& operator<<(std::ostream& out, const DataStruct& value);

#endif
