#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "data_struct.h"

int main() {
    std::vector<DataStruct> vec;
    std::istream_iterator<DataStruct> in_iter(std::cin), eof;
    std::copy(in_iter, eof, std::back_inserter(vec));

    std::sort(vec.begin(), vec.end(),
        [](const DataStruct& a, const DataStruct& b) {
            if (a.key1 != b.key1) return a.key1 < b.key1;
            double mod_a = std::abs(a.key2);
            double mod_b = std::abs(b.key2);
            if (mod_a != mod_b) return mod_a < mod_b;
            return a.key3.size() < b.key3.size();
        });

    std::copy(vec.begin(), vec.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n"));

    return 0;
} 
