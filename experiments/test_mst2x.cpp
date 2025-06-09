#include <bits/stdc++.h>
#include "../src/io.hpp"   
#include "../src/mst2x.hpp"  

std::vector<int> load_opt_tour(const std::string& path) {
    std::ifstream fin(path);
    if (!fin) throw std::runtime_error("Cannot open " + path);

    std::string tok;

    while (fin >> tok && tok != "TOUR_SECTION");
    if (fin.eof()) throw std::runtime_error("TOUR_SECTION not found");

    std::vector<int> tour;
    int v;

    while (fin >> v && v != -1) {
        tour.push_back(v - 1);
    }
    if (!tour.empty())
        tour.push_back(tour.front());
    return tour;
}

long long tour_cost(const std::vector<Point>& pts,
                    const std::vector<int>& tour) {
    long long sum = 0;
    for (size_t i = 1; i < tour.size(); ++i) {
        sum += dist(pts[tour[i-1]], pts[tour[i]]);
    }
    return sum;
}

int main(int argc, char** argv) {
    auto pts = read_tsp(argv[1]);

    auto t0 = std::chrono::high_resolution_clock::now();
    auto tour = mst2x_tour(pts);
    auto t1 = std::chrono::high_resolution_clock::now();

    long long c1 = tour_cost(pts, tour);
    std::chrono::duration<double, std::milli> dt = t1 - t0;

    auto opt_tour = load_opt_tour(argv[2]);
    long long c2 = tour_cost(pts, opt_tour);

    std::cout << "Computed Cost = " << c1 << "\n";
    std::cout << "Optimal cost = " << c2 << "\n";
    std::cout << "Approximation ratio = "
              << std::fixed << std::setprecision(6)
              << (double)c1 / c2 << "\n";
    std::cout << "MST-2X running time = " << std::fixed << std::setprecision(3)
              << dt.count() << " ms\n";

    return 0;
}
