#include "DataStruct.h"
#include <iomanip>

namespace nspace
{
    Iofmtguard::Iofmtguard(std::basic_ios<char>& s)
        : s_(s), width_(s.width()), fill_(s.fill()), precision_(s.precision()), fmt_(s.flags()) {}

    Iofmtguard::~Iofmtguard()
    {
        s_.width(width_);
        s_.fill(fill_);
        s_.precision(precision_);
        s_.flags(fmt_);
    }

    // Чтение разделителя
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
            return in;

        char c;
        in >> c;
        if (in && c != dest.exp)
            in.setstate(std::ios::failbit);
        return in;
    }

    // Чтение восьмиричного числа
    std::istream& operator>>(std::istream& in, OctIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
            return in;

        std::string token;

        char c;
        while (in.get(c))
        {
            if (c == ':' || std::isspace(c))
            {
                in.putback(c);
                break;
            }
            token += c;
        }

        if (!in && token.empty())
            return in;

        if (token.size() >= 2 && token[0] == '0' && (token[1] == 'o' || token[1] == 'O'))
        {
            std::string octStr = token.substr(2);
            for (char ch : octStr)
                if (ch < '0' || ch > '7') {
                    in.setstate(std::ios::failbit);
                    return in;
                }
            dest.ref = std::stoull(octStr, nullptr, 8);
        }
        else
            in.setstate(std::ios::failbit);

        return in;
    }

    // Чтение комплексного числа
    std::istream& operator>>(std::istream& in, ComplexIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
            return in;

        double real, imag;
        in >> DelimiterIO{ '#' } >> DelimiterIO{ 'c' } >> DelimiterIO{ '(' }
        >> real >> imag >> DelimiterIO{ ')' };

        if (in)
            dest.ref = std::complex<double>(real, imag);
        return in;
    }

    // Чтение строки в кавычках
    std::istream& operator>>(std::istream& in, StringIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
            return in;

        return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
    }

    // Чтение и проверка метки
    std::istream& operator>>(std::istream& in, LabelIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
            return in;

        std::string data;
        if ((in >> StringIO{ data }) && data != dest.exp)
            in.setstate(std::ios::failbit);
        return in;
    }

    // Чтение всей структуры
    std::istream& operator>>(std::istream& in, DataStruct& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        DataStruct input;
        bool key1_read = false, key2_read = false, key3_read = false;

        in >> DelimiterIO{ '(' };
        if (!in) return in;

        // Читаем 3 поля (в любом порядке)
        for (int i = 0; i < 3; ++i)
        {
            in >> DelimiterIO{ ':' };
            if (!in) return in;

            std::string label;
            in >> label;
            if (!in) return in;

            if (label == "key1")
            {
                in >> OctIO{ input.key1 };
                key1_read = true;
            }
            else if (label == "key2")
            {
                in >> ComplexIO{ input.key2 };
                key2_read = true;
            }
            else if (label == "key3")
            {
                in >> StringIO{ input.key3 };
                key3_read = true;
            }
            else
            {
                in.setstate(std::ios::failbit);
                return in;
            }

            if (!in) return in;
        }

        // Читаем двоеточие перед закрывающей скобкой
        in >> DelimiterIO{ ':' };
        // Читаем закрывающую скобку
        in >> DelimiterIO{ ')' };

        if (key1_read && key2_read && key3_read)
            dest = std::move(input);
        else
            in.setstate(std::ios::failbit);

        return in;
    }

    // Вывод DataStruct
    std::ostream& operator<<(std::ostream& out, const DataStruct& src)
    {
        Iofmtguard guard(out);

        out << "(:key1 0o" << std::oct << src.key1 << ":"
            << "key2 #c(" << std::fixed << std::setprecision(1)
            << src.key2.real() << " " << src.key2.imag() << "):"
            << "key3 \"" << src.key3 << "\":)";

        return out;
    }
}
