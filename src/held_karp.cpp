#include <vector> 
#include <limits>
#include <algorithm> 
#include "held_karp.hpp" 

#include <iostream>

std::pair<std::vector<int>, long long> held_karp_tour(const std::vector<std::vector<int>>& dist) {
    int n = dist.size();
    int SIZE = 1 << n;

    std::vector<std::vector<long long>> dp(SIZE, std::vector<long long>(n, std::numeric_limits<long long>::max()));
    std::vector<std::vector<int>> parent(SIZE, std::vector<int>(n, -1));

    dp[1][0] = 0; // 시작점은 항상 0번 도시

    for (int mask = 1; mask < SIZE; ++mask) { // mask는 집합을 나타냄
        for (int last = 0; last < n; ++last) { // last는 마지막으로 도달한 정점
            if (!(mask & (1 << last))) continue; // 정점 last가 집합 S에 포함되어 있지 않으면 스킵

            for (int prev = 0; prev < n; ++prev) { // prev는 last에 도달하기 직전에 있던 정점
                if (prev == last || !(mask & (1 << prev))) continue;

                int prev_mask = mask ^ (1 << last);
                if (dp[prev_mask][prev] == std::numeric_limits<long long>::max()) continue;

                long long cost = dp[prev_mask][prev] + dist[prev][last];
                if (cost < dp[mask][last]) {
                    dp[mask][last] = cost;
                    parent[mask][last] = prev;
                }
            }
        }
    }

    long long min_cost = std::numeric_limits<long long>::max();
    int last_node = -1;
    int full_mask = SIZE - 1;
    for (int i = 1; i < n; ++i) {
        long long cost = dp[full_mask][i] + dist[i][0];
        if (cost < min_cost) {
            min_cost = cost;
            last_node = i;
        }
    }

    std::vector<int> path;
    int mask = full_mask, curr = last_node;
    while (curr != -1) {
        path.push_back(curr);
        int temp = parent[mask][curr];
        mask ^= (1 << curr);
        curr = temp;
    }
    std::reverse(path.begin(), path.end());
    path.push_back(0);

    return {path, min_cost};
}
