#include "data_struct.hpp"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <cctype>
#include <algorithm>
#include <stdexcept>

std::string formatULLLit(unsigned long long value) {
    return std::to_string(value) + "ull";
}

std::string formatComplex(const std::complex<double>& value) {
    std::ostringstream oss;
    oss << "#c(" << std::fixed << std::setprecision(1)
        << value.real() << " " << value.imag() << ")";
    return oss.str();
}

bool parseFromString(const std::string& line, DataStruct& dest) {
    if (line.empty() || line.front() != '(' || line.back() != ')') {
        throw std::runtime_error("Invalid format");
    }

    std::string content = line.substr(1, line.size() - 2);
    std::vector<std::string> tokens;
    std::string current;
    bool inQuotes = false;

    for (char c : content) {
        if (c == '"') {
            inQuotes = !inQuotes;
            current += c;
            continue;
        }
        if (!inQuotes && c == ':') {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
            continue;
        }
        current += c;
    }
    if (!current.empty()) {
        tokens.push_back(current);
    }

    DataStruct temp;
    bool hasKey1 = false, hasKey2 = false, hasKey3 = false;

    for (const auto& token : tokens) {
        size_t spacePos = token.find_first_of(" \t");
        if (spacePos == std::string::npos) {
            continue;
        }
        std::string key = token.substr(0, spacePos);
        std::string value = token.substr(spacePos + 1);

        if (key == "key1") {
            std::istringstream iss(value);
            if (iss >> ULLLitIO{ temp.key1 }) {
                hasKey1 = true;
            } else {
                throw std::runtime_error("Failed to parse key1");
            }
        } else if (key == "key2") {
            std::istringstream iss(value);
            if (iss >> ComplexIO{ temp.key2 }) {
                hasKey2 = true;
            } else {
                throw std::runtime_error("Failed to parse key2");
            }
        } else if (key == "key3") {
            std::istringstream iss(value);
            if (iss >> StringIO{ temp.key3 }) {
                hasKey3 = true;
            } else {
                throw std::runtime_error("Failed to parse key3");
            }
        }
    }

    if (hasKey1 && hasKey2 && hasKey3) {
        dest = std::move(temp);
        return true;
    }
    throw std::runtime_error("Missing fields");
}

std::istream& operator>>(std::istream& in, DataStruct& dest) {
    std::string line = "";
    while (std::getline(in, line)) {
        size_t start = line.find_first_not_of(" \t");
        if (start != std::string::npos) {
            size_t end = line.find_last_not_of(" \t");
            line = line.substr(start, end - start + 1);
        }
        try {
            if (parseFromString(line, dest)) {
                return in;
            }
        } catch (const std::exception&) {
            continue;
        }
    }
    in.setstate(std::ios::failbit);
    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& src) {
    std::ostream::sentry sentry(out);
    if (!sentry) {
        return out;
    }
    iofmtguard fmtguard(out);
    out << "(:key1 " << formatULLLit(src.key1)
        << ":key2 " << formatComplex(src.key2)
        << ":key3 \"" << src.key3 << "\":)";
    return out;
}
