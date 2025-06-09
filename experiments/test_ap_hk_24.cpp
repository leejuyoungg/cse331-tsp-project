#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include "../src/io.hpp"
#include "../src/ap_hk.hpp"
#include "../src/held_karp.hpp"

/* 투어 비용 계산 */
static long long tour_cost(const std::vector<Point>& pts,
                           const std::vector<int>& tour)
{
    long long c = 0;
    for (size_t i = 1; i < tour.size(); ++i)
        c += dist(pts[tour[i - 1]], pts[tour[i]]);
    return c;
}

int main(int argc, char** argv)
{
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " input.tsp seed\n";
        return 1;
    }

    const std::string tsp_path = argv[1];
    unsigned base_seed = std::stoul(argv[2]);

    std::vector<Point> full = read_tsp(tsp_path);

    for (int k = 6; k <= 24; ++k)
    {
        if (k > (int)full.size()) {
            std::cerr << "Skipping k=" << k << ": exceeds point count\n";
            continue;
        }

        std::vector<Point> pts = sample_points(full, k, base_seed + k);

        auto [ap_tour, ap_cost] = ap_hk_tour(pts);
        auto [hk_tour, hk_cost] = held_karp_tour(build_distance_matrix(pts));

        if (ap_cost != hk_cost) {
            std::cerr << "❌ MISMATCH at k = " << k << '\n';
            std::cerr << "AP-HK cost = " << ap_cost << ", HK cost = " << hk_cost << '\n';
            return 1;
        } else {
            std::cout << "✅ MATCH at k = " << k << '\n';
        }
    }

    std::cout << "\n🎉 All AP-HK outputs MATCHED Held–Karp from k = 6 to 24.\n";
    return 0;
}
