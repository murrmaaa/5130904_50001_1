#include "data_struct.hpp"

#include <cctype>
#include <iomanip>
#include <ios>
#include <istream>
#include <limits>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace
{
  class StreamFormatGuard
  {
  public:
    explicit StreamFormatGuard(std::basic_ios< char >& stream):
      stream_(stream),
      precision_(stream.precision()),
      fill_(stream.fill()),
      flags_(stream.flags())
    {}

    ~StreamFormatGuard()
    {
      stream_.precision(precision_);
      stream_.fill(fill_);
      stream_.flags(flags_);
    }

  private:
    std::basic_ios< char >& stream_;
    std::streamsize precision_;
    char fill_;
    std::basic_ios< char >::fmtflags flags_;
  };

  void skipWhitespace(const std::string& text, std::size_t& position)
  {
    while ((position < text.size()) &&
        std::isspace(static_cast< unsigned char >(text[position])) != 0)
    {
      ++position;
    }
  }

  bool readRecord(std::istream& in, std::string& record)
  {
    record.clear();

    char symbol = '\0';
    bool insideString = false;
    int depth = 0;

    while (in.get(symbol))
    {
      if (record.empty())
      {
        if (std::isspace(static_cast< unsigned char >(symbol)) != 0)
        {
          continue;
        }
        if (symbol != '(')
        {
          continue;
        }
      }

      record.push_back(symbol);

      if (symbol == '"')
      {
        insideString = !insideString;
      }
      else if (!insideString)
      {
        if (symbol == '(')
        {
          ++depth;
        }
        else if (symbol == ')')
        {
          --depth;
          if (depth == 0)
          {
            return true;
          }
        }
      }
    }

    if (record.empty())
    {
      in.setstate(std::ios::failbit);
    }
    else
    {
      in.setstate(std::ios::failbit);
    }
    return false;
  }

  bool readFieldName(
      const std::string& text,
      std::size_t& position,
      std::string& name)
  {
    const std::size_t start = position;

    while ((position < text.size()) && (text[position] != ' '))
    {
      if (std::isspace(static_cast< unsigned char >(text[position])) != 0)
      {
        return false;
      }
      ++position;
    }

    if ((position == start) || (position >= text.size()) || (text[position] != ' '))
    {
      return false;
    }

    name = text.substr(start, position - start);
    ++position;

    if ((position < text.size()) &&
        std::isspace(static_cast< unsigned char >(text[position])) != 0)
    {
      return false;
    }
    return true;
  }

  bool parseDoubleSci(
      const std::string& text,
      std::size_t& position,
      double& value)
  {
    const std::size_t start = position;

    if ((position < text.size()) &&
        ((text[position] == '+') || (text[position] == '-')))
    {
      ++position;
    }

    const std::size_t intPartStart = position;
    while ((position < text.size()) &&
        std::isdigit(static_cast< unsigned char >(text[position])) != 0)
    {
      ++position;
    }

    if ((position == intPartStart) || (position >= text.size()) ||
        (text[position] != '.'))
    {
      return false;
    }
    ++position;

    const std::size_t fracPartStart = position;
    while ((position < text.size()) &&
        std::isdigit(static_cast< unsigned char >(text[position])) != 0)
    {
      ++position;
    }

    if ((position == fracPartStart) || (position >= text.size()) ||
        ((text[position] != 'e') && (text[position] != 'E')))
    {
      return false;
    }
    ++position;

    if ((position < text.size()) &&
        ((text[position] == '+') || (text[position] == '-')))
    {
      ++position;
    }

    const std::size_t exponentStart = position;
    while ((position < text.size()) &&
        std::isdigit(static_cast< unsigned char >(text[position])) != 0)
    {
      ++position;
    }

    if (position == exponentStart)
    {
      return false;
    }

    try
    {
      value = std::stod(text.substr(start, position - start));
    }
    catch (const std::exception&)
    {
      return false;
    }

    return true;
  }

  bool parseUllBin(
      const std::string& text,
      std::size_t& position,
      unsigned long long& value)
  {
    if ((position + 2 > text.size()) || (text[position] != '0') ||
        ((text[position + 1] != 'b') && (text[position + 1] != 'B')))
    {
      return false;
    }
    position += 2;

    if ((position >= text.size()) ||
        ((text[position] != '0') && (text[position] != '1')))
    {
      return false;
    }

    unsigned long long result = 0;
    const unsigned long long maxValue =
        std::numeric_limits< unsigned long long >::max();

    while ((position < text.size()) &&
        ((text[position] == '0') || (text[position] == '1')))
    {
      const unsigned long long bit =
          static_cast< unsigned long long >(text[position] - '0');

      if (result > ((maxValue - bit) / 2))
      {
        return false;
      }

      result = result * 2 + bit;
      ++position;
    }

    value = result;
    return true;
  }

  bool parseString(
      const std::string& text,
      std::size_t& position,
      std::string& value)
  {
    if ((position >= text.size()) || (text[position] != '"'))
    {
      return false;
    }
    ++position;

    const std::size_t start = position;
    while ((position < text.size()) && (text[position] != '"'))
    {
      ++position;
    }

    if (position >= text.size())
    {
      return false;
    }

    value = text.substr(start, position - start);
    ++position;
    return true;
  }

  bool parseRecord(const std::string& text, DataStruct& value)
  {
    std::size_t position = 0;

    skipWhitespace(text, position);
    if ((position >= text.size()) || (text[position] != '('))
    {
      return false;
    }
    ++position;

    skipWhitespace(text, position);
    if ((position >= text.size()) || (text[position] != ':'))
    {
      return false;
    }
    ++position;

    DataStruct result{};
    bool hasKey1 = false;
    bool hasKey2 = false;
    bool hasKey3 = false;

    while (position < text.size())
    {
      std::string fieldName;
      if (!readFieldName(text, position, fieldName))
      {
        return false;
      }

      bool parsed = false;
      if (fieldName == "key1")
      {
        if (hasKey1)
        {
          return false;
        }
        parsed = parseDoubleSci(text, position, result.key1);
        hasKey1 = parsed;
      }
      else if (fieldName == "key2")
      {
        if (hasKey2)
        {
          return false;
        }
        parsed = parseUllBin(text, position, result.key2);
        hasKey2 = parsed;
      }
      else if (fieldName == "key3")
      {
        if (hasKey3)
        {
          return false;
        }
        parsed = parseString(text, position, result.key3);
        hasKey3 = parsed;
      }
      else
      {
        return false;
      }

      if (!parsed)
      {
        return false;
      }

      if ((position >= text.size()) || (text[position] != ':'))
      {
        return false;
      }
      ++position;

      skipWhitespace(text, position);
      if (position >= text.size())
      {
        return false;
      }
      if (text[position] == ')')
      {
        ++position;
        skipWhitespace(text, position);
        if ((position != text.size()) || !hasKey1 || !hasKey2 || !hasKey3)
        {
          return false;
        }
        value = std::move(result);
        return true;
      }
    }

    return false;
  }

  std::string toBinaryString(unsigned long long value)
  {
    if (value == 0)
    {
      return "0b0";
    }

    std::string bits;
    while (value > 0)
    {
      bits.insert(bits.begin(), static_cast< char >('0' + (value % 2)));
      value /= 2;
    }

    return "0b" + bits;
  }
}

bool compareDataStruct(const DataStruct& lhs, const DataStruct& rhs)
{
  if (lhs.key1 != rhs.key1)
  {
    return lhs.key1 < rhs.key1;
  }
  if (lhs.key2 != rhs.key2)
  {
    return lhs.key2 < rhs.key2;
  }
  return lhs.key3.size() < rhs.key3.size();
}

std::istream& operator>>(std::istream& in, DataStruct& value)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  std::string record;
  DataStruct parsedValue{};

  while (readRecord(in, record))
  {
    if (parseRecord(record, parsedValue))
    {
      value = std::move(parsedValue);
      return in;
    }
  }

  return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& value)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  StreamFormatGuard guard(out);

  out << "(:key1 ";
  out << std::scientific << std::setprecision(1) << value.key1;
  out << ":key2 " << toBinaryString(value.key2);
  out << ":key3 \"" << value.key3 << "\":)";

  return out;
}
