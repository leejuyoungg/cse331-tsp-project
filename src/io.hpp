#pragma once
#include <string>
#include <vector>
#include <cmath>
#include <random>

struct Point {
    double x, y;
};

inline int dist(const Point& a, const Point& b) {
    return static_cast<int>(std::round(std::hypot(a.x - b.x, a.y - b.y)));
}

std::vector<Point> read_tsp(const std::string& path);
std::vector<std::vector<int>> build_distance_matrix(const std::vector<Point>& pts);
std::vector<Point> sample_points(const std::vector<Point>& pts, int k, unsigned seed = std::random_device{}());

