#include "DataStruct.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
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

    if (data.empty())
    {
        std::cout << "Atleast one supported record type" << std::endl;
        return 0;
    }

    std::sort(data.begin(), data.end(),
        [](const DataStruct& a, const DataStruct& b)
        {
            if (a.key1 != b.key1) return a.key1 < b.key1;
            double absA = std::abs(a.key2), absB = std::abs(b.key2);
            const double eps = 1e-10;
            if (std::abs(absA - absB) > eps) return absA < absB;
            return a.key3.length() < b.key3.length();
        }
    );

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}
