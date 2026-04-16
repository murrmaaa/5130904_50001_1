#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>          // std::abs
#include "data_struct.h"

int main() {
    std::vector<DataStruct> data;
    std::string line;

    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        DataStruct ds;
        if (iss >> ds) {
            data.push_back(std::move(ds));
        }
    }

    // Сортировка: key1 → |key2| → длина key3
    std::sort(data.begin(), data.end(), [](const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1) return a.key1 < b.key1;
        double moda = std::abs(a.key2);
        double modb = std::abs(b.key2);
        if (moda != modb) return moda < modb;
        return a.key3.size() < b.key3.size();
    });

    std::copy(data.begin(), data.end(),
              std::ostream_iterator<DataStruct>(std::cout, "\n"));

    return 0;
}
