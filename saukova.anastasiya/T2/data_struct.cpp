#include "data_struct.hpp"
#include <iomanip>
#include <cmath>

namespace anastasiasaukova
{
  std::istream &operator>>(std::istream &in, DelimiterIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    char c = '0';
    in >> c;
    if (in && (std::tolower(c) != std::tolower(dest.expected)))
    {
      in.setstate(std::ios::failbit);
    }
    return in;
  }

  std::istream &operator>>(std::istream &in, DoubleLitIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    return in >> dest.ref >> DelimiterIO{'d'};
  }

  std::istream &operator>>(std::istream &in, LongLongLitIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    return in >> dest.ref >> DelimiterIO{'l'} >> DelimiterIO{'l'};
  }

  std::istream &operator>>(std::istream &in, StringIO &&dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    return std::getline(in >> DelimiterIO{'"'}, dest.ref, '"');
  }

  std::istream &operator>>(std::istream &in, DataStruct &dest)
  {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    DataStruct input{0.0, 0, ""};
    bool h1 = false, h2 = false, h3 = false;
    if (!(in >> DelimiterIO{'('} >> DelimiterIO{':'})) return in;

    for (int i = 0; i < 3; ++i)
    {
      std::string key = "";
      in >> key;
      if (key == "key1" && !h1)
      {
        in >> DoubleLitIO{input.key1_};
        h1 = true;
      }
      else if (key == "key2" && !h2)
      {
        in >> LongLongLitIO{input.key2_};
        h2 = true;
      }
      else if (key == "key3" && !h3)
      {
        in >> StringIO{input.key3_};
        h3 = true;
      }
      in >> DelimiterIO{':'};
    }
    if (in >> DelimiterIO{')'}) dest = input;
    return in;
  }

  std::ostream &operator<<(std::ostream &out, const DataStruct &src)
  {
    std::ostream::sentry sentry(out);
    if (!sentry) return out;
    iofmtguard guard(out);
    out << "(:key1 " << std::fixed << std::setprecision(1) << src.key1_ << "d";
    out << ":key2 " << src.key2_ << "ll";
    out << ":key3 \"" << src.key3_ << "\":)";
    return out;
  }

  bool compareDataStruct(const DataStruct &lhs, const DataStruct &rhs)
  {
    if (std::abs(lhs.key1_ - rhs.key1_) > 1e-9) return lhs.key1_ < rhs.key1_;
    if (lhs.key2_ != rhs.key2_) return lhs.key2_ < rhs.key2_;
    return lhs.key3_.length() < rhs.key3_.length(); // Вариант 6: длина строки
  }

  iofmtguard::iofmtguard(std::basic_ios< char > &s) :
    s_(s), width_(s.width()), fill_(s.fill()), precision_(s.precision()), fmt_(s.flags()) {}
  iofmtguard::~iofmtguard() { s_.width(width_); s_.fill(fill_); s_.precision(precision_); s_.flags(fmt_); }
}
