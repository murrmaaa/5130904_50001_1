#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>
#include "data_struct.h"

struct Line {
    std::string str;
};

std::istream& operator>>(std::istream& in, Line& line) {
    return std::getline(in, line.str);
}

int main() {
    std::vector<DataStruct> vec;
    std::istream_iterator<Line> in_iter(std::cin), eof;
    std::for_each(in_iter, eof, [&](const Line& line) {
        std::istringstream iss(line.str);
        DataStruct temp;
        if (iss >> temp) {
            vec.push_back(temp);
        }
    });

    if (vec.empty()) {
        std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped\n";
    } else {
        std::cout << "Atleast one supported record type\n";
    }

    return 0;
}
