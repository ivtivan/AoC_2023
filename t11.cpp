#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

auto part_1(std::ifstream& input) -> uint64_t {
    std::string line;
    std::vector<std::string> data;
    std::vector<std::pair<int, int>> galaxies;
    std::vector<uint> empty_cols;

    uint counter {0};
    int sum_distances{0};
    while (std::getline(input, line)) {
        if (galaxies.empty()) {
            empty_cols.assign(line.size(), 0);
            std::iota(empty_cols.begin(), empty_cols.end(), 0);
        }
        auto pos = line.find('#');
        if (pos == std::string::npos) {
            ++counter;
        }
        while (pos != std::string::npos) {
            galaxies.emplace_back(counter, pos);
            auto found = std::find(empty_cols.begin(), empty_cols.end(), pos);
            if (found != empty_cols.end()) {
                empty_cols.erase(found);
            }
            pos = line.find('#', pos + 1);
        }
        ++counter;
    }

    for (uint i = 0; i < galaxies.size() - 1; ++i) {
        for (uint j = i + 1; j < galaxies.size(); ++j) {
            sum_distances += std::abs(galaxies[j].first - galaxies[i].first) + std::abs(galaxies[j].second - galaxies[i].second);

            sum_distances += std::abs(std::lower_bound(empty_cols.begin(), empty_cols.end(), std::max(galaxies[j].second, galaxies[i].second)) - 
                std::lower_bound(empty_cols.begin(), empty_cols.end(), std::min(galaxies[j].second, galaxies[i].second)));
        }
    }
    return sum_distances;
}

auto part_2(std::ifstream& input) -> uint64_t {
    std::string line;
    std::vector<std::string> data;
    std::vector<std::pair<int, int>> galaxies;
    std::vector<uint> empty_cols;

    uint64_t counter {0};
    uint64_t sum_distances{0};
    while (std::getline(input, line)) {
        if (galaxies.empty()) {
            empty_cols.assign(line.size(), 0);
            std::iota(empty_cols.begin(), empty_cols.end(), 0);
        }
        auto pos = line.find('#');
        if (pos == std::string::npos) {
            counter += 999'999ull;
        }
        while (pos != std::string::npos) {
            galaxies.emplace_back(counter, pos);
            auto found = std::find(empty_cols.begin(), empty_cols.end(), pos);
            if (found != empty_cols.end()) {
                empty_cols.erase(found);
            }
            pos = line.find('#', pos + 1);
        }
        ++counter;
    }

    for (uint i = 0; i < galaxies.size() - 1; ++i) {
        for (uint j = i + 1; j < galaxies.size(); ++j) {
            sum_distances += std::abs(galaxies[j].first - galaxies[i].first) + std::abs(galaxies[j].second - galaxies[i].second);

            sum_distances += 999'999ull * std::abs(std::lower_bound(empty_cols.begin(), empty_cols.end(), std::max(galaxies[j].second, galaxies[i].second)) - 
                std::lower_bound(empty_cols.begin(), empty_cols.end(), std::min(galaxies[j].second, galaxies[i].second)));
        }
    }
    return sum_distances;
}


int main() {
    std::ifstream input("inputs/AoC_11.txt");
    // std::cout << "Part 1: " << part_1(input) << std::endl;
    std::cout << "Part 2: " << part_2(input) << std::endl;
}