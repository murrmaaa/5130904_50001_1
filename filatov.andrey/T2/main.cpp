#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>
#include "data_struct.h"

bool tryParse(const std::string& line, DataStruct& out) {
    std::istringstream iss(line);
    return !!(iss >> out);
}

int main() {
    std::vector<DataStruct> vec;
    std::istream_iterator<std::string> in_iter(std::cin), eof;
    std::for_each(in_iter, eof, [&](const std::string& line) {
        DataStruct temp;
        if (tryParse(line, temp)) {
            vec.push_back(temp);
        }
    });

    if (vec.empty()) {
        std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped\n";
        return 0;
    }

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

    std::cout << "Atleast one supported record type\n";

    return 0;
}
