
#include "DataStruct.hpp"
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
    explicit StreamFormatGuard(std::basic_ios< char >& stream) :
      stream_(stream),
      precision_(stream.precision()),
      fill_(stream.fill()),
      flags_(stream.flags())
    {
    }

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
        std::isspace(static_cast<unsigned char>(text[position])) != 0)
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
        if (std::isspace(static_cast<unsigned char>(symbol)) != 0)
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

    in.setstate(std::ios::failbit);
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
      if (std::isspace(static_cast<unsigned char>(text[position])) != 0)
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
        std::isspace(static_cast<unsigned char>(text[position])) != 0)
    {
      return false;
    }
    return true;
  }

  bool parseSllLit(
      const std::string& text,
      std::size_t& position,
      long long& value)
  {
    const std::size_t start = position;

    if ((position < text.size()) && (text[position] == '-'))
    {
      ++position;
    }

    const std::size_t digitsStart = position;
    while ((position < text.size()) &&
        std::isdigit(static_cast<unsigned char>(text[position])) != 0)
    {
      ++position;
    }

    if ((position == digitsStart) || (position + 1 >= text.size()))
    {
      return false;
    }

    std::string suffix = text.substr(position, 2);
    if (suffix != "LL" && suffix != "ll")
    {
      return false;
    }
    position += 2;

    try
    {
      value = std::stoll(text.substr(start, position - start - 2));
    }
    catch (const std::exception&)
    {
      return false;
    }

    return true;
  }

  bool parseRational(
      const std::string& text,
      std::size_t& position,
      std::pair<long long, unsigned long long>& value)
  {
    if ((position >= text.size()) || (text[position] != '('))
    {
      return false;
    }
    ++position;

    if ((position >= text.size()) || (text[position] != ':'))
    {
      return false;
    }
    ++position;

    if ((position >= text.size()) || (text[position] != 'N'))
    {
      return false;
    }
    ++position;

    skipWhitespace(text, position);

    long long numerator = 0;
    const std::size_t numStart = position;
    if ((position < text.size()) && (text[position] == '-'))
    {
      ++position;
    }
    while ((position < text.size()) &&
        std::isdigit(static_cast<unsigned char>(text[position])) != 0)
    {
      ++position;
    }
    if (position == numStart)
    {
      return false;
    }
    try
    {
      numerator = std::stoll(text.substr(numStart, position - numStart));
    }
    catch (const std::exception&)
    {
      return false;
    }

    if ((position >= text.size()) || (text[position] != ':'))
    {
      return false;
    }
    ++position;

    if ((position >= text.size()) || (text[position] != 'D'))
    {
      return false;
    }
    ++position;

    skipWhitespace(text, position);

    unsigned long long denominator = 0;
    const std::size_t denStart = position;
    while ((position < text.size()) &&
        std::isdigit(static_cast<unsigned char>(text[position])) != 0)
    {
      ++position;
    }
    if (position == denStart)
    {
      return false;
    }
    try
    {
      denominator = std::stoull(text.substr(denStart, position - denStart));
    }
    catch (const std::exception&)
    {
      return false;
    }

    if ((position >= text.size()) || (text[position] != ':'))
    {
      return false;
    }
    ++position;

    if ((position >= text.size()) || (text[position] != ')'))
    {
      return false;
    }
    ++position;

    if (denominator == 0)
    {
      return false;
    }

    value = { numerator, denominator };
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
        parsed = parseSllLit(text, position, result.key1);
        hasKey1 = parsed;
      }
      else if (fieldName == "key2")
      {
        if (hasKey2)
        {
          return false;
        }
        parsed = parseRational(text, position, result.key2);
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

  std::string formatLongLongLit(long long value)
  {
    return std::to_string(value) + "ll";
  }

  std::string formatRational(const std::pair<long long, unsigned long long>& value)
  {
    return "(:N " + std::to_string(value.first) + ":D " + std::to_string(value.second) + ":)";
  }
}

bool compareDataStruct(const DataStruct& lhs, const DataStruct& rhs)
{
  if (lhs.key1 != rhs.key1)
  {
    return lhs.key1 < rhs.key1;
  }

  double ra = static_cast<double>(lhs.key2.first) / lhs.key2.second;
  double rb = static_cast<double>(rhs.key2.first) / rhs.key2.second;

  if (ra != rb)
  {
    return ra < rb;
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

  out << "(:key1 " << formatLongLongLit(value.key1)
      << ":key2 " << formatRational(value.key2)
      << ":key3 \"" << value.key3 << "\":)";

  return out;
}

