#include <bits/stdc++.h>
#include "../src/io.hpp"
#include "../src/mst2x.hpp"
#include "../src/held_karp.hpp"
#include <chrono>

long long tour_cost(const std::vector<Point>& pts, const std::vector<int>& tour) {
    long long cost = 0;
    for (size_t i = 1; i < tour.size(); ++i) {
        cost += dist(pts[tour[i - 1]], pts[tour[i]]);
    }
    return cost;
}

int main(int argc, char** argv) {
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0]
                  << " input.tsp k trials seed\n";
        return 1;
    }

    std::string tsp_path = argv[1];
    int k = std::stoi(argv[2]);
    int trials = std::stoi(argv[3]);
    unsigned base_seed = std::stoul(argv[4]);

    auto full_pts = read_tsp(tsp_path);

    if (k > (int)full_pts.size()) {
        std::cerr << "Error: k = " << k << " > n = " << full_pts.size() << "\n";
        return 1;
    }

    double total_ratio = 0.0;
    double total_mst_time = 0.0;
    double total_hk_time = 0.0;

    for (int t = 0; t < trials; ++t) {
        auto pts = sample_points(full_pts, k, base_seed + t);

        // MST-2× 실행 시간 측정
        auto t0 = std::chrono::high_resolution_clock::now();
        auto mst_tour = mst2x_tour(pts);
        auto t1 = std::chrono::high_resolution_clock::now();

        // Held-Karp 실행 시간 측정
        auto t2 = std::chrono::high_resolution_clock::now();
        auto [hk_tour, hk_cost] = held_karp_tour(build_distance_matrix(pts));
        auto t3 = std::chrono::high_resolution_clock::now();

        double mst_time = std::chrono::duration<double, std::milli>(t1 - t0).count();
        double hk_time = std::chrono::duration<double, std::milli>(t3 - t2).count();

        total_mst_time += mst_time;
        total_hk_time += hk_time;

        double ratio = (double)tour_cost(pts, mst_tour) / hk_cost;
        total_ratio += ratio;
    }

    std::cout << tsp_path << ", k=" << k << ", trials=" << trials << "\n";
    std::cout << "  avg MST2X/HK ratio = "
              << std::fixed << std::setprecision(6)
              << (total_ratio / trials) << "\n";
    std::cout << "  avg MST2X runtime = "
              << std::fixed << std::setprecision(3)
              << (total_mst_time / trials) << " ms\n";
    std::cout << "  avg Held-Karp runtime = "
              << std::fixed << std::setprecision(3)
              << (total_hk_time / trials) << " ms\n";

    return 0;
}
