
#include "format.hpp"
#include <string>
#include <utility>

std::string formatLongLongLit(long long value)
{
  return std::to_string(value) + "LL";
}

std::string formatRational(const std::pair<long long, unsigned long long>& value)
{
  return "(:N " + std::to_string(value.first) + ":D " + std::to_string(value.second) + ":)";
}

