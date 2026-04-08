
/*
#include "parsers.hpp"
#include "DataStruct.hpp"
#include <cctype>
#include <istream>
#include <string>
#include <utility>

namespace
{
  void skipWhitespace(const std::string& text, std::size_t& position)
  {
    while ((position < text.size()) &&
        std::isspace(static_cast<unsigned char>(text[position])) != 0)
    {
      ++position;
    }
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
*/

