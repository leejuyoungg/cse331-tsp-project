/*  check/test_ap_hk.cpp  ─────────────────────────────────────────────
 *  usage:  ./test_ap_hk  input.tsp   k   trials   seed
 *          (k ≥ 25 권장,   trials ≥ 1)
 *  ------------------------------------------------------------------ */

#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include "../src/io.hpp"          // Point, read_tsp(), dist()
#include "../src/ap_hk.hpp"       // ap_hk_tour()
#include "../src/mst2x.hpp"       // (옵션) 정확성 체크용
#include "../src/held_karp.hpp"   // (k < 25 일 때만) 정확성 비교

/* ─── 보조: 투어 비용 계산 ─────────────────────────────────────────── */
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
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0]
                  << " input.tsp  k  trials  seed\n";
        return 1;
    }

    const std::string tsp_path = argv[1];
    int k       = std::stoi(argv[2]);
    int trials  = std::stoi(argv[3]);
    unsigned seed0 = std::stoul(argv[4]);

    std::vector<Point> full = read_tsp(tsp_path);
    if (k > (int)full.size()) {
        std::cerr << "Error: k = " << k << " > n = " << full.size() << '\n';
        return 1;
    }
    if (k < 2) { std::cerr << "k must be ≥ 2\n"; return 1; }

    double total_ms   = 0.0;
    long long bestCost = -1;

    for (int t = 0; t < trials; ++t)
    {
        std::vector<Point> pts = sample_points(full, k, seed0 + t);

        auto t0 = std::chrono::high_resolution_clock::now();
        auto [tour, cost] = ap_hk_tour(pts);
        auto t1 = std::chrono::high_resolution_clock::now();

        /* sanity: k < 25 이면 기존 Held-Karp로 cost 확인 */
        if (k < 25) {
            auto distM = build_distance_matrix(pts);
            auto [tourHK, costHK] = held_karp_tour(distM);
            if (cost != costHK) {
                std::cerr << "❌ mismatch on trial " << t << '\n';
                return 1;
            }
            else {
                std::cout << "✅ MATCH!" <<'\n';
            }
        }

        std::chrono::duration<double, std::milli> dt = t1 - t0;
        total_ms += dt.count();
        if (bestCost == -1 || cost < bestCost) bestCost = cost;

        std::cout << "trial " << t
                  << "  time = " << std::fixed << std::setprecision(1)
                  << dt.count() << " ms"
                  << "  cost = " << cost << '\n';
    }

    std::cout << "\n[" << tsp_path << "]  k = " << k
              << ", trials = " << trials << '\n'
              << "avg runtime = " << std::fixed << std::setprecision(2)
              << (total_ms / trials) << " ms\n"
              << "best cost   = " << bestCost << '\n';

    std::cout << "✅ AP-HK finished all trials without memory errors.\n";
    return 0;
}
