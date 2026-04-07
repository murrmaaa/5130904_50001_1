#include "dataIO.hpp"
#include "dataStruct.hpp"
#include "iofmtguard.hpp"
#include <iostream>
#include <iomanip>

using Ull = unsigned long long;

std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {

    std::istream::sentry sentry(in);

    if (!sentry) {
        return in;
    }

    char c = '0';

    in >> c;

    if (in && (c != dest.expected)) {
        in.setstate(std::ios::failbit);
    }

    return in;
}

std::istream& operator>>(std::istream& in, DoubleIO&& dest) {

    std::istream::sentry sentry(in);

    if (!sentry) {
        return in;
    }

    std::string buffer;

    while (std::isdigit(in.peek())
        || in.peek() == '.'
        || in.peek() == '-') {

        buffer += static_cast<char>(in.get());
    }

    size_t dotPos = buffer.find('.');

    if (dotPos == std::string::npos ||
        dotPos == 0 || !std::isdigit(buffer[dotPos - 1]) ||
        dotPos == buffer.size() - 1 || !std::isdigit(buffer[dotPos + 1])) {

        in.setstate(std::ios::failbit);
        return in;
    }

    char lit = '0';

    in >> lit;

    if (!in || (lit != 'd' && lit != 'D')) {
        in.setstate(std::ios::failbit);
        return in;
    }

    if (in) {
        dest.ref = std::stod(buffer);
    }

    return in;
}

std::istream& operator>>(std::istream& in, StringIO&& dest) {

    std::istream::sentry sentry(in);

    if (!sentry) {
        return in;
    }

    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}

std::istream& operator>>(std::istream& in, UllIO&& dest) {

    std::istream::sentry sentry(in);

    if (!sentry) {
        return in;
    }

    iofmtguard guard(in);

    return in >> std::hex >> dest.ref;
}

std::istream& operator>>(std::istream& in, DataStruct& dest) {

    std::istream::sentry sentry(in);

    if (!sentry) {
        return in;
    }

    DataStruct input;

    {
        using sep = DelimiterIO;
        using dbl = DoubleIO;
        using str = StringIO;
        using ull = UllIO;

        if (!(in >> sep{ '(' } >> sep{ ':' })) {
            in.setstate(std::ios::failbit);
            return in;
        }

        for (int i = 0; i < 3; ++i) {

            in >> sep{ 'k' } >> sep{ 'e' } >> sep{ 'y' };

            char keyNum = in.get();

            if (keyNum == '1') {
                in >> dbl{ input.key1 };
            }
            else if (keyNum == '2') {
                in >> ull{ input.key2 };
            }
            else if (keyNum == '3') {
                in >> str{ input.key3 };
            }
            else {
                in.setstate(std::ios::failbit);
            }

            if (i < 2) {
                in >> sep{ ':' };
            }
        }

        in >> sep{ ':' } >> sep{ ')' };
    }


    if (in) {
        dest = std::move(input);
    }

    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& dest) {

    std::ostream::sentry sentry(out);

    if (!sentry) {
        return out;
    }

    out << "(:key1 ";

    {
        iofmtguard guard(out);

        out << std::fixed << std::setprecision(1) << dest.key1;
    }

    out << "d:key2 ";

    {
        iofmtguard guard(out);

        out << "0x" << std::hex << std::uppercase << std::noshowbase << dest.key2;
    }

    out << ":key3 \"" << dest.key3 << "\":)\n";

    return out;
}
