#include "io_types.hpp"
#include <sstream>
#include <cmath>
#include <cctype>

std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    char c;
    in >> c;
    if (in && c != dest.exp) {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, ULLHexIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    std::string token;
    in >> token;
    if (!in) {
        return in;
    }
    if (token.size() < 3 || token[0] != '0' || (token[1] != 'x' && token[1] != 'X')) {
        in.setstate(std::ios::failbit);
        return in;
    }
    std::string hexStr = token.substr(2);
    if (hexStr.empty()) {
        in.setstate(std::ios::failbit);
        return in;
    }
    for (char c : hexStr) {
        if (!std::isxdigit(static_cast<unsigned char>(c))) {
            in.setstate(std::ios::failbit);
            return in;
        }
    }
    try {
        dest.ref = std::stoull(hexStr, nullptr, 16);
        return in;
    } catch (...) {
        in.setstate(std::ios::failbit);
        return in;
    }
}

std::istream& operator>>(std::istream& in, ComplexIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    std::string token;
    in >> token;
    if (!in) {
        return in;
    }
    if (token.size() < 6 || token[0] != '#' || token[1] != 'c' || token[2] != '(') {
        in.setstate(std::ios::failbit);
        return in;
    }
    std::string content = token.substr(3, token.size() - 4);
    size_t spacePos = content.find_first_of(" \t");
    if (spacePos == std::string::npos) {
        in.setstate(std::ios::failbit);
        return in;
    }
    std::string realStr = content.substr(0, spacePos);
    std::string imagStr = content.substr(spacePos + 1);
    try {
        double real = std::stod(realStr);
        double imag = std::stod(imagStr);
        dest.ref = std::complex<double>(real, imag);
        return in;
    } catch (...) {
        in.setstate(std::ios::failbit);
        return in;
    }
}

std::istream& operator>>(std::istream& in, StringIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}

std::istream& operator>>(std::istream& in, LabelIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    std::string label;
    if ((in >> StringIO{ label }) && label != dest.exp) {
        in.setstate(std::ios::failbit);
    }
    return in;
}
