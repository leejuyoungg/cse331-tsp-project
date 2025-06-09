#include "io.hpp"
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <numeric>

std::vector<Point> read_tsp(const std::string& path) {
    std::ifstream fin(path);
    if (!fin) throw std::runtime_error("Cannot open " + path);

    std::string tok;
    int dim = -1;

    while (fin >> tok) {
        if (tok == "DIMENSION:" || tok == "DIMENSION" || tok == "DIMENSION :") { fin >> dim; }
        if (tok == "NODE_COORD_SECTION") break;
    }
    if (dim <= 0) throw std::runtime_error("DIMENSION not found");
    if (tok != "NODE_COORD_SECTION")          
        throw std::runtime_error("NODE_COORD_SECTION not found");

    std::vector<Point> pts(dim);
    int id; double x, y;
    while (fin >> id) {         
        if (!(fin >> x >> y)) break;
        int idx = id -1;
        if (idx < 0 || idx >= dim)
            throw std::runtime_error("Node id out of range");
        pts[idx] = {x, y};
    }
    return pts;
}

std::vector<Point> sample_points(const std::vector<Point>& pts, int k, unsigned seed) {
    if (k > (int)pts.size())
        throw std::runtime_error("Sample size exceeds number of cities");

    std::vector<int> indices(pts.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), std::mt19937(seed));

    std::vector<Point> sampled;
    for (int i = 0; i < k; ++i) {
        sampled.push_back(pts[indices[i]]);
    }
    return sampled;
}

std::vector<std::vector<int>> build_distance_matrix(const std::vector<Point>& pts)
{
    int n = (int)pts.size();
    std::vector<std::vector<int>> D(n, std::vector<int>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            D[i][j] = (i == j ? 0 : (int)dist(pts[i], pts[j]));
    return D;
}
