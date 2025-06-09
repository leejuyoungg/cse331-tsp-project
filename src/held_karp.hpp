#pragma once
#include <vector>
#include <utility>
// 비용 같이 반환할지 말지 고민
std::pair<std::vector<int>, long long> held_karp_tour(const std::vector<std::vector<int>>& dist);