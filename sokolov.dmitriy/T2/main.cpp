// Variant 8
// key1: DBL LIT key2: ULL HEX

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <iterator>
#include "dataStruct.hpp"
#include "dataIO.hpp"
#include "dataStructComparator.hpp"

int main() {

    std::vector<DataStruct> data;

    while (!std::cin.eof()) {

        DataStruct temp;

        if (std::cin >> temp) {
            data.push_back(temp);
        }
        else {
            if (std::cin.eof()) {
                break;
            }

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::sort(std::begin(data), std::end(data), dataStructComparator);

    std::copy(
        std::begin(data),
        std::end(data),
        std::ostream_iterator<DataStruct>(std::cout)
    );

    return EXIT_SUCCESS;
}
