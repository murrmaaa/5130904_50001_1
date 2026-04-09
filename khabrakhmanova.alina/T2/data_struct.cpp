#include "data_struct.hpp"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cctype>
#include <stdexcept>
#include <algorithm>

std::string formatHexULL(unsigned long long value) {
    std::ostringstream oss;
    oss << "0x" << std::hex << std::uppercase << value;
    return oss.str();
}

std::string formatComplex(const std::complex<double>& value) {
    std::ostringstream oss;
    oss << "#c(" << std::fixed << std::setprecision(1)
        << value.real() << " " << value.imag() << ")";
    return oss.str();
}

std::istream& operator>>(std::istream& in, DataStruct& dest) {
    std::string line;
    if (!std::getline(in, line)) {
        in.setstate(std::ios::failbit);
        return in;
    }

    try {
        DataStruct temp;

        // Находим key1
        size_t key1_pos = line.find("key1");
        if (key1_pos == std::string::npos) throw std::runtime_error("key1 not found");

        size_t colon1 = line.find(":", key1_pos);
        if (colon1 == std::string::npos) throw std::runtime_error("colon after key1 not found");

        size_t hex_start = line.find("0x", colon1);
        if (hex_start == std::string::npos) throw std::runtime_error("0x not found");

        size_t hex_end = hex_start + 2;
        while (hex_end < line.size() && std::isxdigit(static_cast<unsigned char>(line[hex_end]))) {
            hex_end++;
        }

        std::string hex_str = line.substr(hex_start + 2, hex_end - hex_start - 2);
        if (hex_str.empty()) throw std::runtime_error("empty hex");
        temp.key1 = std::stoull(hex_str, nullptr, 16);

        // Находим key2
        size_t key2_pos = line.find("key2");
        if (key2_pos == std::string::npos) throw std::runtime_error("key2 not found");

        size_t colon2 = line.find(":", key2_pos);
        if (colon2 == std::string::npos) throw std::runtime_error("colon after key2 not found");

        size_t complex_start = line.find("#c(", colon2);
        if (complex_start == std::string::npos) throw std::runtime_error("#c( not found");

        size_t paren_open = complex_start + 3;
        size_t paren_close = line.find(")", paren_open);
        if (paren_close == std::string::npos) throw std::runtime_error(") not found");

        std::string complex_content = line.substr(paren_open, paren_close - paren_open);

        // Парсим real и imag через stringstream
        std::stringstream ss(complex_content);
        double real, imag;
        if (!(ss >> real >> imag)) {
            throw std::runtime_error("failed to parse complex numbers");
        }
        temp.key2 = std::complex<double>(real, imag);

        // Находим key3
        size_t key3_pos = line.find("key3");
        if (key3_pos == std::string::npos) throw std::runtime_error("key3 not found");

        size_t colon3 = line.find(":", key3_pos);
        if (colon3 == std::string::npos) throw std::runtime_error("colon after key3 not found");

        size_t quote1 = line.find('"', colon3);
        if (quote1 == std::string::npos) throw std::runtime_error("opening quote not found");

        size_t quote2 = line.find('"', quote1 + 1);
        if (quote2 == std::string::npos) throw std::runtime_error("closing quote not found");

        temp.key3 = line.substr(quote1 + 1, quote2 - quote1 - 1);

        dest = temp;
        return in;

    } catch (const std::exception&) {
        in.setstate(std::ios::failbit);
        return in;
    }
}

std::ostream& operator<<(std::ostream& out, const DataStruct& src) {
    out << "(:key1 " << formatHexULL(src.key1)
        << ":key2 " << formatComplex(src.key2)
        << ":key3 \"" << src.key3 << "\":)";
    return out;
}
