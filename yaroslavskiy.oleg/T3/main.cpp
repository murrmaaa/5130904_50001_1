
// Yaroslavsky Oleg 5130904 / 50001 T3

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>
#include <iomanip>
#include <cmath>
#include <set>

using namespace std::placeholders;

struct Point {

    int x, y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }

};

struct Polygon {

    std::vector<Point> points;

    bool operator==(const Polygon& other) const {
        if (points.size() != other.points.size()) return false;
        std::multiset<Point> set1(points.begin(), points.end());
        std::multiset<Point> set2(other.points.begin(), other.points.end());
        return set1 == set2;
    }

    bool exactMatch(const Polygon& other) const {
        if (points.size() != other.points.size()) return false;
        return std::equal(points.begin(), points.end(), other.points.begin());
    }

};

double triangleArea(const Point& a, const Point& b, const Point& c) {
    double ab = std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
    double bc = std::sqrt((c.x - b.x) * (c.x - b.x) + (c.y - b.y) * (c.y - b.y));
    double ca = std::sqrt((a.x - c.x) * (a.x - c.x) + (a.y - c.y) * (a.y - c.y));
    double p = (ab + bc + ca) / 2.0;
    return std::sqrt(p * (p - ab) * (p - bc) * (p - ca));
}

double polygonArea(const Polygon& poly) {
    if (poly.points.size() < 3) return 0.0;
    double area = 0.0;
    const Point& p0 = poly.points[0];
    for (size_t i = 2; i < poly.points.size(); ++i) {
        area += triangleArea(p0, poly.points[i - 1], poly.points[i]);
    }
    return area;
}

std::vector<Polygon> readPolygons(const std::string& filename) {
    std::vector<Polygon> polygons;
    std::ifstream file(filename);
    if (!file.is_open()) return polygons;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        int vertexCount;
        if (!(iss >> vertexCount)) continue;
        if (vertexCount < 3) continue;

        Polygon poly;
        bool valid = true;

        for (int i = 0; i < vertexCount; ++i) {
            char openParen, semicolon, closeParen;
            int x, y;
            if (!(iss >> openParen >> x >> semicolon >> y >> closeParen) ||
                openParen != '(' || semicolon != ';' || closeParen != ')') {
                valid = false;
                break;
            }
            poly.points.push_back({ x, y });
        }

        std::string remaining;
        std::getline(iss, remaining);
        if (remaining.find_first_not_of(" \t\r") != std::string::npos) {
            valid = false;
        }

        if (valid && poly.points.size() == static_cast<size_t>(vertexCount)) {
            polygons.push_back(poly);
        }
    }
    return polygons;
}

bool parsePolygon(const std::string& str, Polygon& poly) {
    std::istringstream iss(str);
    int vertexCount;
    if (!(iss >> vertexCount)) return false;
    if (vertexCount < 3) return false;

    poly.points.clear();
    for (int i = 0; i < vertexCount; ++i) {
        char openParen, semicolon, closeParen;
        int x, y;
        if (!(iss >> openParen >> x >> semicolon >> y >> closeParen) ||
            openParen != '(' || semicolon != ';' || closeParen != ')') {
            return false;
        }
        poly.points.push_back({ x, y });
    }

    std::string remaining;
    std::getline(iss, remaining);
    if (remaining.find_first_not_of(" \t\r") != std::string::npos) {
        return false;
    }
    return poly.points.size() == static_cast<size_t>(vertexCount);
}

bool isValidVertexCount(int n) {
    return n >= 3;
}

void processCommands(std::vector<Polygon>& polygons) {
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string command;
        iss >> command;


        if (command == "COUNT") {
            std::string param;
            iss >> param;
            if (param == "EVEN") {
                std::cout << std::count_if(polygons.begin(), polygons.end(),
                    [](const Polygon& p) { return p.points.size() % 2 == 0; }) << std::endl;
            }
            else if (param == "ODD") {
                std::cout << std::count_if(polygons.begin(), polygons.end(),
                    [](const Polygon& p) { return p.points.size() % 2 == 1; }) << std::endl;
            }
            else {
                int vertexCount = std::stoi(param);
                if (!isValidVertexCount(vertexCount)) {
                    std::cout << "<INVALID COMMAND>" << std::endl;
                    continue;
                }
                std::cout << std::count_if(polygons.begin(), polygons.end(),
                    [vertexCount](const Polygon& p) { return p.points.size() == static_cast<size_t>(vertexCount); }) << std::endl;
            }
        }


        else if (command == "AREA") {
            std::string param;
            iss >> param;
            if (param == "MEAN") {
                if (polygons.empty()) {
                    std::cout << "<INVALID COMMAND>" << std::endl;
                }
                else {
                    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                        [](double acc, const Polygon& p) { return acc + polygonArea(p); });
                    std::cout << std::fixed << std::setprecision(1) << (sum / polygons.size()) << std::endl;
                }
            }
            else if (param == "EVEN") {
                double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                    [](double acc, const Polygon& p) {
                        return acc + (p.points.size() % 2 == 0 ? polygonArea(p) : 0.0);
                    });
                std::cout << std::fixed << std::setprecision(1) << sum << std::endl;
            }
            else if (param == "ODD") {
                double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                    [](double acc, const Polygon& p) {
                        return acc + (p.points.size() % 2 == 1 ? polygonArea(p) : 0.0);
                    });
                std::cout << std::fixed << std::setprecision(1) << sum << std::endl;
            }
            else {
                int vertexCount = std::stoi(param);
                if (!isValidVertexCount(vertexCount)) {
                    std::cout << "<INVALID COMMAND>" << std::endl;
                    continue;
                }
                double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                    [vertexCount](double acc, const Polygon& p) {
                        return acc + (p.points.size() == static_cast<size_t>(vertexCount) ? polygonArea(p) : 0.0);
                    });
                std::cout << std::fixed << std::setprecision(1) << sum << std::endl;
            }

        }

        else if (command == "MAX") {
            std::string param;
            iss >> param;
            if (polygons.empty()) {
                std::cout << "<INVALID COMMAND>" << std::endl;
                continue;
            }
            if (param == "AREA") {
                auto maxIt = std::max_element(polygons.begin(), polygons.end(),
                    [](const Polygon& a, const Polygon& b) { return polygonArea(a) < polygonArea(b); });
                std::cout << std::fixed << std::setprecision(1) << polygonArea(*maxIt) << std::endl;
            }
            else if (param == "VERTEXES") {
                auto maxIt = std::max_element(polygons.begin(), polygons.end(),
                    [](const Polygon& a, const Polygon& b) { return a.points.size() < b.points.size(); });
                std::cout << maxIt->points.size() << std::endl;
            }
            else {
                std::cout << "<INVALID COMMAND>" << std::endl;
            }
        }

        else if (command == "MIN") {
            std::string param;
            iss >> param;
            if (polygons.empty()) {
                std::cout << "<INVALID COMMAND>" << std::endl;
                continue;
            }
            if (param == "AREA") {
                auto minIt = std::min_element(polygons.begin(), polygons.end(),
                    [](const Polygon& a, const Polygon& b) { return polygonArea(a) < polygonArea(b); });
                std::cout << std::fixed << std::setprecision(1) << polygonArea(*minIt) << std::endl;
            }
            else if (param == "VERTEXES") {
                auto minIt = std::min_element(polygons.begin(), polygons.end(),
                    [](const Polygon& a, const Polygon& b) { return a.points.size() < b.points.size(); });
                std::cout << minIt->points.size() << std::endl;
            }
            else {
                std::cout << "<INVALID COMMAND>" << std::endl;

            }
        }


        else if (command == "PERMS") {
            std::string polygonStr;
            std::getline(iss, polygonStr);
            size_t start = polygonStr.find_first_not_of(" \t");
            if (start != std::string::npos) polygonStr = polygonStr.substr(start);
            Polygon target;
            if (!parsePolygon(polygonStr, target)) {
                std::cout << "<INVALID COMMAND>" << std::endl;
                continue;
            }
            std::cout << std::count_if(polygons.begin(), polygons.end(),
                std::bind(std::equal_to<Polygon>(), _1, target)) << std::endl;
        }


        else if (command == "MAXSEQ") {
            std::string polygonStr;
            std::getline(iss, polygonStr);
            size_t start = polygonStr.find_first_not_of(" \t");
            if (start != std::string::npos) polygonStr = polygonStr.substr(start);
            Polygon target;
            if (!parsePolygon(polygonStr, target)) {
                std::cout << "<INVALID COMMAND>" << std::endl;
                continue;
            }
            int maxLen = 0, currentLen = 0;
            for (const auto& poly : polygons) {
                if (poly.exactMatch(target)) {
                    currentLen++;
                    if (currentLen > maxLen) maxLen = currentLen;
                }
                else {
                    currentLen = 0;
                }
            }
            std::cout << maxLen << std::endl;
        }
        else {
            std::cout << "<INVALID COMMAND>" << std::endl;
        }

    }

}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    if (argc != 2) {
        std::cerr << "Ошибка: не указан файл с фигурами" << std::endl;
        return EXIT_FAILURE;
    }
    std::string filename = argv[1];
    std::vector<Polygon> polygons = readPolygons(filename);
    processCommands(polygons);
    return EXIT_SUCCESS;

}


