#pragma once
#include <vector>
#include "io.hpp"

/*struct for mst_prim_heap*/
/*struct Edge { int to; double w; };
using Adj = std::vector<std::vector<Edge>>; */

std::vector<int> mst2x_tour(const std::vector<Point>& pts);
std::vector<std::vector<int>> mst_prim_linear(const std::vector<Point>& pts, const int r);
void dfs_pre(int u, int p, const std::vector<std::vector<int>>& adj, std::vector<int>& order);
//std::vector<std::vector<int>> mst_prim_heap(const std::vector<Point>& pts, const int r);