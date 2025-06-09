#include <vector>
#include "mst2x.hpp"
//#include "min_heap.hpp"

std::vector<int> mst2x_tour(const std::vector<Point>& pts) {
    int r = 0; /* index of a root vertex */
    std::vector<std::vector<int>> mst = mst_prim_linear(pts, 0);
    std::vector<int> full_walk;
    dfs_pre(0, -1, mst, full_walk);

    // 단축
    int n = pts.size();
    std::vector<int> tour;
    std::vector<char> visited(n,0);
    for(int v : full_walk) {
        if (!visited[v]) {
            tour.push_back(v);
            visited[v] = 1;
        }
    }
    tour.push_back(r);
    return tour;
}

std::vector<std::vector<int>> mst_prim_linear(const std::vector<Point>& pts, const int r) { //이름 적절?
    int n = pts.size();
    const double INF = std::numeric_limits<double>::infinity();
    std::vector<double> key(n, INF);
    std::vector<int> parent(n, -1); // -1 아직 배정되지 않았다는 의미?
    std::vector<bool> in_mst(n, false);

    key[r] = 0;
    for (int iter = 0; iter < n; ++iter) { // iter 반복 횟수 나타내는 것? MST 구하는 거니까 결과적으로 모든 정점 포함해야 하는 거니까 iter < n 즉 n번 반복?
        int u = -1; // -1 아직 배정되지 않았다는 의미?
        double best = INF;
        for (int i = 0; i < n; ++i) {
            if (!in_mst[i] && key[i] < best) { 
                best = key[i]; 
                u = i; 
            } // mst에 안 속해 있고 거리(key)가 best보다 작으면 u = i
        }
        in_mst[u] = true; // mst에 u 포함시키기

        for (int v = 0; v < n; ++v) { // u 포함시킨 다음 mst에 속하지 않는 정점들에 대해서 u와의 거리 계산, 비교 후 거리(key) 업데이트되면 parent u로 갱신
            if (!in_mst[v]) {
                double w = dist(pts[u], pts[v]);
                if (w < key[v]) { 
                    key[v] = w; 
                    parent[v] = u; 
                }
            }
        }
    }

    std::vector<std::vector<int>> adj(n);
    for (int v = 1; v < n; ++v) {
        int u = parent[v]; // parent로 서로 연결되어 있는 정점 파악, 각 정점의 adj 리스트에 포함
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    return adj;
}

void dfs_pre(int u, int p, const std::vector<std::vector<int>>& adj, std::vector<int>& order) {
    order.push_back(u);
    for (int v : adj[u]) {
        if (v != p) {
            dfs_pre(v, u, adj, order);
        }
    }
}

/*std::vector<std::vector<int>> mst_prim_heap(const std::vector<Point>& pts, const int r) {
    const int n = static_cast<int>(pts.size());
    const double INF = std::numeric_limits<double>::infinity();

    Minheap pq(n, INF);
    std::vector<double> key(n, INF);
    std::vector<int> parent(n, -1);

    key[r] = 0;
    pq.push(r, 0);

    while (!pq.empty()) {
        int u = pq.extract_min();

        for (int v = 0; v < n; ++v) {
            if (u == v) continue;
            double w = dist(pts[u], pts[v]);

            if (pq.contains(v) && w < key[v]) { // contains 아직 구현은 안 했지만 MST에 속하지 않은 정점인지 판별하는 로직
                key[v] = w;
                parent[v] = u;
                pq.decrease_key(v, w);
            }
        }
    }

    std::vector<std::vector<int>> adj(n);
    for (int v = 1; v < n; ++v) {
        int u = parent[v];
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    return adj;
}*/