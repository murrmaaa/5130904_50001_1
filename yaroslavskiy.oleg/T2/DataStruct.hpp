
#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <iosfwd>
#include <string>
#include <utility>

struct DataStruct
{
  long long key1;
  std::pair<long long, unsigned long long> key2;
  std::string key3;
};

bool compareDataStruct(const DataStruct& lhs, const DataStruct& rhs);

std::istream& operator>>(std::istream& in, DataStruct& value);
std::ostream& operator<<(std::ostream& out, const DataStruct& value);

#endif



