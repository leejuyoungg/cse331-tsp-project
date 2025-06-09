#pragma once
#include "io.hpp"
#include <vector> 
#include <cstdint> 
using Mask = std::uint64_t; 
using Cost = long long; 
Cost prim_mst_cost(const std::vector<std::vector<int>>& D, const std::vector<int>& R);
Cost lower_bound(const std::vector<std::vector<int>>& D, Mask mask, int last, const std::vector<int>& min0);
std::pair<std::vector<int>, Cost> ap_hk_tour(const std::vector<Point>&);
std::vector<int> mst2x_2opt_tour(const std::vector<Point>& pts);