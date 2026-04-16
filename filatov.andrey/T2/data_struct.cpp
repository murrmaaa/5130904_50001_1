#include "data_struct.h"
#include <sstream>
#include <iomanip>
#include <string>
#include <algorithm>  // для std::transform (tolower)

std::istream& operator>>(std::istream& is, DataStruct& ds) {
    std::string record;
    if (!std::getline(is, record)) {
        is.setstate(std::ios::failbit);
        return is;
    }

    if (record.size() < 5 || record.substr(0, 2) != "(:") || record.substr(record.size() - 2) != ":)") {
        is.setstate(std::ios::failbit);
        return is;
    }

    std::string inner = record.substr(2, record.size() - 4);

    auto extract_value = [&](const std::string& k) -> std::string {
        std::string key_str = k + " ";
        size_t pos = inner.find(":" + key_str);
        if (pos == std::string::npos) {
            pos = inner.find(key_str);
            if (pos == std::string::npos) return "";
        } else {
            pos += 1;
        }
        size_t val_start = pos + key_str.length();
        if (val_start >= inner.size()) return "";

        if (inner[val_start] == '"') {
            size_t close_quote = val_start + 1;
            while (close_quote < inner.size() && inner[close_quote] != '"') ++close_quote;
            if (close_quote >= inner.size() || inner[close_quote] != '"') return "";
            return inner.substr(val_start, close_quote - val_start + 1);
        } else {
            size_t next_colon = inner.find(':', val_start);
            if (next_colon == std::string::npos) {
                return inner.substr(val_start);
            }
            return inner.substr(val_start, next_colon - val_start);
        }
    };

    std::string v1 = extract_value("key1");
    std::string v2 = extract_value("key2");
    std::string v3 = extract_value("key3");

    if (v1.empty() || v2.empty() || v3.empty()) {
        is.setstate(std::ios::failbit);
        return is;
    }

    // === key1: SLL LIT ===
    {
        if (v1.size() < 3) {
            is.setstate(std::ios::failbit);
            return is;
        }
        std::string suffix = v1.substr(v1.size() - 2);
        if (suffix != "ll" && suffix != "LL") {
            is.setstate(std::ios::failbit);
            return is;
        }
        std::string num_str = v1.substr(0, v1.size() - 2);
        std::istringstream iss_num(num_str);
        if (!(iss_num >> ds.key1)) {
            is.setstate(std::ios::failbit);
            return is;
        }
        std::string extra;
        if (iss_num >> extra) {
            is.setstate(std::ios::failbit);
            return is;
        }
    }

    // === key2: CMP LSP ===
    {
        std::string val = v2;
        if (val.size() < 5 || val.substr(0, 3) != "#c(" || val.back() != ')') {
            is.setstate(std::ios::failbit);
            return is;
        }
        std::string inside = val.substr(3, val.size() - 4);
        std::istringstream iss(inside);
        double re, im;
        if (!(iss >> re >> im)) {
            is.setstate(std::ios::failbit);
            return is;
        }
        std::string extra;
        if (iss >> extra) {
            is.setstate(std::ios::failbit);
            return is;
        }
        ds.key2 = std::complex<double>(re, im);
    }

    // === key3: строка ===
    {
        std::string val = v3;
        if (val.size() < 2 || val.front() != '"' || val.back() != '"') {
            is.setstate(std::ios::failbit);
            return is;
        }
        ds.key3 = val.substr(1, val.size() - 2);
    }

    return is;
}

std::ostream& operator<<(std::ostream& os, const DataStruct& ds) {
    os << "(:key1 " << ds.key1 << "ll:key2 #c("
       << std::fixed << std::setprecision(1)
       << ds.key2.real() << " " << ds.key2.imag()
       << "):key3 \"" << ds.key3 << "\":)";
    return os;
}
