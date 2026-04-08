#include "data_struct.hpp"
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

int main()
{
    using namespace nspace;

    std::vector<DataStruct> data;

    std::copy(
        std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(data)
    );

    std::sort(data.begin(), data.end(), compareData);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return EXIT_SUCCESS;
}
