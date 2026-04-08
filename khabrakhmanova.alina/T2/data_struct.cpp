#include "data_struct.hpp"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <vector>

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

bool parseFromString(const std::string& line, DataStruct& dest) {
    if (line.empty() || line.front() != '(' || line.back() != ')') {
        return false;
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
            if (iss >> ULLHexIO{ temp.key1 }) {
                hasKey1 = true;
            }
        } else if (key == "key2") {
            std::istringstream iss(value);
            if (iss >> ComplexIO{ temp.key2 }) {
                hasKey2 = true;
            }
        } else if (key == "key3") {
            std::istringstream iss(value);
            if (iss >> StringIO{ temp.key3 }) {
                hasKey3 = true;
            }
        }
    }

    if (hasKey1 && hasKey2 && hasKey3) {
        dest = std::move(temp);
        return true;
    }
    return false;
}

std::istream& operator>>(std::istream& in, DataStruct& dest) {
    std::string line = "";
    while (std::getline(in, line)) {
        size_t start = line.find_first_not_of(" \t");
        if (start != std::string::npos) {
            size_t end = line.find_last_not_of(" \t");
            line = line.substr(start, end - start + 1);
        }
        if (parseFromString(line, dest)) {
            return in;
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
    out << "(:key1 " << formatHexULL(src.key1)
        << ":key2 " << formatComplex(src.key2)
        << ":key3 \"" << src.key3 << "\":)";
    return out;
}
