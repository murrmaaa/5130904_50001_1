#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

namespace t3
{
  struct Point
  {
    int x;
    int y;
  };

  struct Polygon
  {
    std::vector< Point > points;
  };

  struct Frame
  {
    int minX;
    int maxX;
    int minY;
    int maxY;
    bool initialized;
  };

  class Iofmtguard
  {
  public:
    explicit Iofmtguard(std::basic_ios< char >& stream):
      stream_(stream),
      width_(stream.width()),
      fill_(stream.fill()),
      precision_(stream.precision()),
      flags_(stream.flags())
    {}

    ~Iofmtguard()
    {
      stream_.width(width_);
      stream_.fill(fill_);
      stream_.precision(precision_);
      stream_.flags(flags_);
    }

  private:
    std::basic_ios< char >& stream_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags flags_;
  };

  namespace
  {
    const std::size_t MIN_VERTEX_COUNT = 3;
    const char* INVALID_COMMAND = "<INVALID COMMAND>";

    bool isDigit(char symbol)
    {
      return std::isdigit(static_cast< unsigned char >(symbol)) != 0;
    }

    bool readExactSymbol(const std::string& text, std::size_t& position, char symbol)
    {
      if ((position >= text.size()) || (text[position] != symbol))
      {
        return false;
      }
      ++position;
      return true;
    }

    bool readUnsignedNumber(
      const std::string& text,
      std::size_t& position,
      std::size_t& number
    )
    {
      const auto begin = text.cbegin() + static_cast< std::ptrdiff_t >(position);
      const auto end = std::find_if_not(begin, text.cend(), isDigit);
      if (begin == end)
      {
        return false;
      }

      try
      {
        const std::string token(begin, end);
        number = static_cast< std::size_t >(std::stoull(token));
        position = static_cast< std::size_t >(std::distance(text.cbegin(), end));
      }
      catch (...)
      {
        return false;
      }
      return true;
    }

    bool readSignedNumber(const std::string& text, std::size_t& position, int& number)
    {
      const std::size_t start = position;
      if ((position < text.size()) && (text[position] == '-'))
      {
        ++position;
      }

      const auto begin = text.cbegin() + static_cast< std::ptrdiff_t >(position);
      const auto end = std::find_if_not(begin, text.cend(), isDigit);
      if (begin == end)
      {
        position = start;
        return false;
      }

      try
      {
        const std::size_t endPosition = static_cast< std::size_t >(
          std::distance(text.cbegin(), end)
        );
        const std::string token = text.substr(start, endPosition - start);
        const long long value = std::stoll(token);
        if ((value < std::numeric_limits< int >::min()) ||
          (value > std::numeric_limits< int >::max()))
        {
          position = start;
          return false;
        }
        number = static_cast< int >(value);
        position = endPosition;
      }
      catch (...)
      {
        position = start;
        return false;
      }
      return true;
    }

    bool readPointFromText(const std::string& text, std::size_t& position, Point& point)
    {
      Point readPoint{ 0, 0 };
      const std::size_t start = position;
      const bool valid =
        readExactSymbol(text, position, '(') &&
        readSignedNumber(text, position, readPoint.x) &&
        readExactSymbol(text, position, ';') &&
        readSignedNumber(text, position, readPoint.y) &&
        readExactSymbol(text, position, ')');

      if (!valid)
      {
        position = start;
        return false;
      }
      point = readPoint;
      return true;
    }

    bool readPointsFromText(
      const std::string& text,
      std::size_t& position,
      std::size_t count,
      std::vector< Point >& points
    )
    {
      if (count == 0)
      {
        return position == text.size();
      }

      Point point{ 0, 0 };
      if (!readExactSymbol(text, position, ' ') || !readPointFromText(text, position, point))
      {
        return false;
      }

      points.push_back(point);
      return readPointsFromText(text, position, count - 1, points);
    }

    long long getCrossProduct(const Point& left, const Point& right)
    {
      return static_cast< long long >(left.x) * right.y -
        static_cast< long long >(right.x) * left.y;
    }

    long long getDoubleAreaSum(const std::vector< Point >& points, std::size_t index)
    {
      if (index == points.size())
      {
        return 0;
      }
      const Point& current = points[index];
      const Point& next = points[(index + 1) % points.size()];
      return getCrossProduct(current, next) + getDoubleAreaSum(points, index + 1);
    }

    Frame addPointToFrame(Frame frame, const Point& point)
    {
      if (!frame.initialized)
      {
        return Frame{ point.x, point.x, point.y, point.y, true };
      }

      frame.minX = std::min(frame.minX, point.x);
      frame.maxX = std::max(frame.maxX, point.x);
      frame.minY = std::min(frame.minY, point.y);
      frame.maxY = std::max(frame.maxY, point.y);
      return frame;
    }

    Frame addPolygonToFrame(Frame frame, const Polygon& polygon)
    {
      return std::accumulate(
        polygon.points.cbegin(),
        polygon.points.cend(),
        frame,
        addPointToFrame
      );
    }

    bool isPointInsideFrame(const Point& point, const Frame& frame)
    {
      return frame.initialized &&
        (frame.minX <= point.x) && (point.x <= frame.maxX) &&
        (frame.minY <= point.y) && (point.y <= frame.maxY);
    }

    void printPoints(
      std::ostream& output,
      std::vector< Point >::const_iterator current,
      std::vector< Point >::const_iterator end
    )
    {
      if (current == end)
      {
        return;
      }

      output << ' ' << '(' << current->x << ';' << current->y << ')';
      printPoints(output, std::next(current), end);
    }

    bool parsePolygonLine(const std::string& line, Polygon& polygon)
    {
      std::size_t position = 0;
      std::size_t vertexCount = 0;
      if (!readUnsignedNumber(line, position, vertexCount) || (vertexCount < MIN_VERTEX_COUNT))
      {
        return false;
      }

      std::vector< Point > points;
      points.reserve(vertexCount);
      if (!readPointsFromText(line, position, vertexCount, points))
      {
        return false;
      }

      polygon.points = points;
      return true;
    }

    std::istream& readNextValidPolygon(std::istream& input, Polygon& polygon)
    {
      std::string line;
      if (!std::getline(input, line))
      {
        input.setstate(std::ios::failbit);
        return input;
      }

      if (parsePolygonLine(line, polygon))
      {
        return input;
      }

      return readNextValidPolygon(input, polygon);
    }

    bool isPointLess(const Point& left, const Point& right)
    {
      return (left.x < right.x) || ((left.x == right.x) && (left.y < right.y));
    }


    double getArea(const Polygon& polygon)
    {
      const long long doubleArea = getDoubleAreaSum(polygon.points, 0);
      return std::abs(static_cast< double >(doubleArea)) / 2.0;
    }

    bool isPolygonPerm(const Polygon& left, const Polygon& right)
    {
      if (left.points.size() != right.points.size())
      {
        return false;
      }

      std::vector< Point > leftPoints = left.points;
      std::vector< Point > rightPoints = right.points;
      std::sort(leftPoints.begin(), leftPoints.end(), isPointLess);
      std::sort(rightPoints.begin(), rightPoints.end(), isPointLess);
      return std::equal(
        leftPoints.cbegin(),
        leftPoints.cend(),
        rightPoints.cbegin(),
        [](const Point& leftPoint, const Point& rightPoint)
        {
          return (leftPoint.x == rightPoint.x) && (leftPoint.y == rightPoint.y);
        }
      );
    }

    Frame getFrame(const std::vector< Polygon >& polygons)
    {
      return std::accumulate(
        polygons.cbegin(),
        polygons.cend(),
        Frame{ 0, 0, 0, 0, false },
        addPolygonToFrame
      );
    }

    bool isPolygonInsideFrame(const Polygon& polygon, const Frame& frame)
    {
      return std::all_of(
        polygon.points.cbegin(),
        polygon.points.cend(),
        [&frame](const Point& point)
        {
          return isPointInsideFrame(point, frame);
        }
      );
    }

    void printInvalidCommand(std::ostream& output)
    {
      output << INVALID_COMMAND << '\n';
    }

    void printDouble(std::ostream& output, double value)
    {
      Iofmtguard guard(output);
      output << std::fixed << std::setprecision(1) << value << '\n';
    }

    bool hasNoExtraData(std::istream& input)
    {
      input >> std::ws;
      return input.eof();
    }

    bool parseVertexCount(const std::string& text, std::size_t& vertexCount)
    {
      if (text.empty() || !std::all_of(text.cbegin(), text.cend(), isDigit))
      {
        return false;
      }

      try
      {
        vertexCount = static_cast< std::size_t >(std::stoull(text));
      }
      catch (...)
      {
        return false;
      }

      return vertexCount >= MIN_VERTEX_COUNT;
    }

    bool isEvenVertexCount(const Polygon& polygon)
    {
      return polygon.points.size() % 2 == 0;
    }

    bool isOddVertexCount(const Polygon& polygon)
    {
      return !isEvenVertexCount(polygon);
    }

    bool hasVertexCount(const Polygon& polygon, std::size_t vertexCount)
    {
      return polygon.points.size() == vertexCount;
    }

    template< class Predicate >
    double getAreaSum(const std::vector< Polygon >& polygons, Predicate predicate)
    {
      return std::accumulate(
        polygons.cbegin(),
        polygons.cend(),
        0.0,
        [&predicate](double sum, const Polygon& polygon)
        {
          return predicate(polygon) ? sum + getArea(polygon) : sum;
        }
      );
    }

    double getTotalArea(const std::vector< Polygon >& polygons)
    {
      return getAreaSum(
        polygons,
        [](const Polygon&)
        {
          return true;
        }
      );
    }

    bool parsePolygonArgument(std::istream& input, Polygon& polygon)
    {
      std::string line;
      std::getline(input >> std::ws, line);
      return !line.empty() && parsePolygonLine(line, polygon);
    }

    bool executeArea(
      std::istream& input,
      const std::vector< Polygon >& polygons,
      std::ostream& output
    )
    {
      std::string parameter;
      if (!(input >> parameter) || !hasNoExtraData(input))
      {
        return false;
      }

      if (parameter == "EVEN")
      {
        printDouble(output, getAreaSum(polygons, isEvenVertexCount));
        return true;
      }
      if (parameter == "ODD")
      {
        printDouble(output, getAreaSum(polygons, isOddVertexCount));
        return true;
      }
      if (parameter == "MEAN")
      {
        if (polygons.empty())
        {
          return false;
        }
        printDouble(output, getTotalArea(polygons) / polygons.size());
        return true;
      }

      std::size_t vertexCount = 0;
      if (!parseVertexCount(parameter, vertexCount))
      {
        return false;
      }
      printDouble(
        output,
        getAreaSum(
          polygons,
          [vertexCount](const Polygon& polygon)
          {
            return hasVertexCount(polygon, vertexCount);
          }
        )
      );
      return true;
    }

    bool executeMax(
      std::istream& input,
      const std::vector< Polygon >& polygons,
      std::ostream& output
    )
    {
      std::string parameter;
      if (!(input >> parameter) || !hasNoExtraData(input) || polygons.empty())
      {
        return false;
      }

      if (parameter == "AREA")
      {
        const auto result = std::max_element(
          polygons.cbegin(),
          polygons.cend(),
          [](const Polygon& left, const Polygon& right)
          {
            return getArea(left) < getArea(right);
          }
        );
        printDouble(output, getArea(*result));
        return true;
      }
      if (parameter == "VERTEXES")
      {
        const auto result = std::max_element(
          polygons.cbegin(),
          polygons.cend(),
          [](const Polygon& left, const Polygon& right)
          {
            return left.points.size() < right.points.size();
          }
        );
        output << result->points.size() << '\n';
        return true;
      }

      return false;
    }

    bool executeMin(
      std::istream& input,
      const std::vector< Polygon >& polygons,
      std::ostream& output
    )
    {
      std::string parameter;
      if (!(input >> parameter) || !hasNoExtraData(input) || polygons.empty())
      {
        return false;
      }

      if (parameter == "AREA")
      {
        const auto result = std::min_element(
          polygons.cbegin(),
          polygons.cend(),
          [](const Polygon& left, const Polygon& right)
          {
            return getArea(left) < getArea(right);
          }
        );
        printDouble(output, getArea(*result));
        return true;
      }
      if (parameter == "VERTEXES")
      {
        const auto result = std::min_element(
          polygons.cbegin(),
          polygons.cend(),
          [](const Polygon& left, const Polygon& right)
          {
            return left.points.size() < right.points.size();
          }
        );
        output << result->points.size() << '\n';
        return true;
      }

      return false;
    }

    bool executeCount(
      std::istream& input,
      const std::vector< Polygon >& polygons,
      std::ostream& output
    )
    {
      std::string parameter;
      if (!(input >> parameter) || !hasNoExtraData(input))
      {
        return false;
      }

      if (parameter == "EVEN")
      {
        output << std::count_if(polygons.cbegin(), polygons.cend(), isEvenVertexCount) << '\n';
        return true;
      }
      if (parameter == "ODD")
      {
        output << std::count_if(polygons.cbegin(), polygons.cend(), isOddVertexCount) << '\n';
        return true;
      }

      std::size_t vertexCount = 0;
      if (!parseVertexCount(parameter, vertexCount))
      {
        return false;
      }
      output << std::count_if(
        polygons.cbegin(),
        polygons.cend(),
        [vertexCount](const Polygon& polygon)
        {
          return hasVertexCount(polygon, vertexCount);
        }
      ) << '\n';
      return true;
    }

    bool executePerms(
      std::istream& input,
      const std::vector< Polygon >& polygons,
      std::ostream& output
    )
    {
      Polygon polygon;
      if (!parsePolygonArgument(input, polygon))
      {
        return false;
      }

      output << std::count_if(
        polygons.cbegin(),
        polygons.cend(),
        [&polygon](const Polygon& current)
        {
          return isPolygonPerm(current, polygon);
        }
      ) << '\n';
      return true;
    }

    bool executeInframe(
      std::istream& input,
      const std::vector< Polygon >& polygons,
      std::ostream& output
    )
    {
      Polygon polygon;
      if (polygons.empty() || !parsePolygonArgument(input, polygon))
      {
        return false;
      }

      const bool result = isPolygonInsideFrame(polygon, getFrame(polygons));
      output << (result ? "<TRUE>" : "<FALSE>") << '\n';
      return true;
    }

    void executeCommand(
      const std::string& line,
      const std::vector< Polygon >& polygons,
      std::ostream& output
    )
    {
      std::istringstream input(line);
      std::string command;
      bool executed = false;

      if (input >> command)
      {
        if (command == "AREA")
        {
          executed = executeArea(input, polygons, output);
        }
        else if (command == "MAX")
        {
          executed = executeMax(input, polygons, output);
        }
        else if (command == "MIN")
        {
          executed = executeMin(input, polygons, output);
        }
        else if (command == "COUNT")
        {
          executed = executeCount(input, polygons, output);
        }
        else if (command == "PERMS")
        {
          executed = executePerms(input, polygons, output);
        }
        else if (command == "INFRAME")
        {
          executed = executeInframe(input, polygons, output);
        }
      }

      if (!executed)
      {
        printInvalidCommand(output);
      }
    }

    void processCommands(
      std::istream& input,
      std::ostream& output,
      const std::vector< Polygon >& polygons
    )
    {
      std::string line;
      if (!std::getline(input, line))
      {
        return;
      }

      executeCommand(line, polygons, output);
      processCommands(input, output, polygons);
    }
  }

  std::istream& operator>>(std::istream& input, Polygon& polygon)
  {
    if (!input)
    {
      return input;
    }
    return readNextValidPolygon(input, polygon);
  }

  void run(std::istream& input, std::ostream& output, const std::vector< Polygon >& polygons)
  {
    processCommands(input, output, polygons);
  }
}

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cerr << "Filename is not specified\n";
    return 1;
  }

  std::ifstream input(argv[1]);
  if (!input)
  {
    std::cerr << "Cannot open file\n";
    return 1;
  }

  std::vector< t3::Polygon > polygons;
  std::copy(
    std::istream_iterator< t3::Polygon >(input),
    std::istream_iterator< t3::Polygon >(),
    std::back_inserter(polygons)
  );

  t3::run(std::cin, std::cout, polygons);
  return 0;
}
