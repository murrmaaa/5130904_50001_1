
// T2 Yaroslavsky Oleg 5130904 / 50001 Varian 11

#include "DataStruct.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main()
{
  std::vector< DataStruct > data;

  std::copy(
    std::istream_iterator< DataStruct >(std::cin),
    std::istream_iterator< DataStruct >(),
    std::back_inserter(data));

  std::sort(data.begin(), data.end(), compareDataStruct);

  std::copy(
    data.cbegin(),
    data.cend(),
    std::ostream_iterator< DataStruct >(std::cout, "\n"));
}

