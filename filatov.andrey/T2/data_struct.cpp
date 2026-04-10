#include "data_struct.h"
#include <sstream>
#include <string>
#include <iomanip>
#include <cctype>

static std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\n\r");
    return s.substr(start, end - start + 1);
}

static std::string extractValue(const std::string& str, const std::string& key) {
    std::string search = ":" + key;
    size_t pos = str.find(search);
    if (pos == std::string::npos) return "";
    pos += search.length();
    while (pos < str.size() && std::isspace(static_cast<unsigned char>(str[pos]))) ++pos;
    size_t end = str.find(':', pos);
    if (end == std::string::npos) end = str.size();
    return trim(str.substr(pos, end - pos));
}

std::istream& operator>>(std::istream& in, DataStruct& data) {
    std::string line;
    if (!std::getline(in, line)) {
        in.setstate(std::ios::failbit);
        return in;
    }
    line = trim(line);
    if (line.empty()) {
        in.setstate(std::ios::failbit);
        return in;
    }
    if (line.size() >= 2 && line.front() == '(' && line.back() == ')') {
        line = line.substr(1, line.size() - 2);
        line = trim(line);
    }
    std::string key1_str = extractValue(line, "key1");
    std::string key2_str = extractValue(line, "key2");
    std::string key3_str = extractValue(line, "key3");

    if (key1_str.empty() || key2_str.empty() || key3_str.empty()) {
        in.setstate(std::ios::failbit);
        return in;
    }

    // key1: удаляем суффикс ll/LL
    std::string k1 = key1_str;
    if (k1.size() >= 2 &&
        (k1.back() == 'L' || k1.back() == 'l') &&
        (k1[k1.size()-2] == 'L' || k1[k1.size()-2] == 'l')) {
        k1.pop_back();
        k1.pop_back();
    }
    try {
        data.key1 = std::stoll(k1);
    } catch (...) {
        in.setstate(std::ios::failbit);
        return in;
    }

    // key2: #c(real imag)
    if (key2_str.size() < 5 || key2_str.substr(0,3) != "#c(" || key2_str.back() != ')') {
        in.setstate(std::ios::failbit);
        return in;
    }
    std::string numbers = key2_str.substr(3, key2_str.size() - 4);
    std::istringstream iss(numbers);
    double real, imag;
    if (!(iss >> real >> imag)) {
        in.setstate(std::ios::failbit);
        return in;
    }
    data.key2 = std::complex<double>(real, imag);

    // key3: "string"
    if (key3_str.size() < 2 || key3_str.front() != '"' || key3_str.back() != '"') {
        in.setstate(std::ios::failbit);
        return in;
    }
    data.key3 = key3_str.substr(1, key3_str.size() - 2);

    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& data) {
    out << "(:key1 " << data.key1;
    out << ":key2 #c(" << std::fixed << std::setprecision(1)
        << data.key2.real() << " " << data.key2.imag() << ")";
    out << ":key3 \"" << data.key3 << "\":)";
    return out;
}
