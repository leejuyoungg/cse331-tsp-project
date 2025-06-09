#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include "../src/io.hpp"
#include "../src/ap_hk.hpp"
#include "../src/mst2x.hpp"

long long tour_cost(const std::vector<Point>& pts, const std::vector<int>& tour) {
    long long cost = 0;
    for (size_t i = 1; i < tour.size(); ++i)
        cost += dist(pts[tour[i - 1]], pts[tour[i]]);
    return cost;
}

int main(int argc, char** argv) {
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " input.tsp k trials seed\n";
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
    double total_mst_time = 0.0, total_aphk_time = 0.0;

    for (int t = 0; t < trials; ++t) {
        std::cout << "Trial " << t << ": sampling k = " << k << " with seed = " << base_seed + t << "...\n";
        auto pts = sample_points(full_pts, k, base_seed + t);

        std::cout << "  Running MST2X...\n";
        auto t0 = std::chrono::high_resolution_clock::now();
        auto mst_tour = mst2x_tour(pts);
        auto t1 = std::chrono::high_resolution_clock::now();

        std::cout << "  Running AP-HK...\n";
        auto [aphk_tour, aphk_cost] = ap_hk_tour(pts);
        auto t2 = std::chrono::high_resolution_clock::now();

        long long mst_cost = tour_cost(pts, mst_tour);
        double ratio = (double)mst_cost / aphk_cost;
        total_ratio += ratio;

        double mst_time = std::chrono::duration<double, std::milli>(t1 - t0).count();
        double aphk_time = std::chrono::duration<double, std::milli>(t2 - t1).count();
        total_mst_time += mst_time;
        total_aphk_time += aphk_time;

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "    MST2X time: " << mst_time << " ms, cost: " << mst_cost << '\n';
        std::cout << "    AP-HK  time: " << aphk_time << " ms, cost: " << aphk_cost << '\n';
        std::cout << "    MST2X/AP-HK ratio: " << std::setprecision(6) << ratio << "\n\n";
    }

    std::cout << "[Comparison: MST2X vs. AP-HK] " << tsp_path
              << ", k = " << k << ", trials = " << trials << '\n'
              << "avg MST2X/AP-HK ratio = " << std::fixed << std::setprecision(6)
              << (total_ratio / trials) << '\n'
              << "avg MST2X time = " << std::setprecision(2) << (total_mst_time / trials) << " ms\n"
              << "avg AP-HK  time = " << (total_aphk_time / trials) << " ms\n";

    return 0;
}
