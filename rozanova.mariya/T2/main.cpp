#include "DataStruct.h"
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <cmath>

int main()
{
    using nspace::DataStruct;

    std::vector<DataStruct> data;

    std::copy(
        std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(data)
    );

    std::sort(data.begin(), data.end(),
        [](const DataStruct& a, const DataStruct& b)
        {
            if (a.key1 != b.key1) return a.key1 < b.key1;
            double absA = std::abs(a.key2), absB = std::abs(b.key2);
            if (absA != absB) return absA < absB;
            return a.key3.length() < b.key3.length();
        }
    );

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return EXIT_SUCCESS;
}
