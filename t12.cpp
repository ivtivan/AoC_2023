#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

auto tokenize(const std::string& data) -> std::vector<short> {
    std::vector<short> tokens;
    auto pos {data.find(',')};
    auto prev{0};
    while (pos != std::string::npos) {
        tokens.push_back(static_cast<short>(std::stoi(data.substr(prev, pos - prev))));
        prev = pos + 1;
        pos = data.find(',', prev);
    }
    tokens.push_back(static_cast<short>(std::stoi(data.substr(prev))));

    return tokens;
}

//  returns a bit mask
auto get_unknown(const std::string& springs) -> uint32_t {
    uint32_t unknown{0};
    for (auto& spring : springs) {
        unknown = (unknown << 1);
        if (spring == '?') {
            ++unknown;
        }
    }

    return unknown;
}

auto get_consecutive(const std::string& springs) -> std::vector<short> {
    std::vector<short> consecutive;
    bool counting = false;
    short curr{0};
    for (const auto el : springs) {
        if (el == '#') {
            if (!counting) counting = true;
            ++curr;
        }
        else if (counting) {
                consecutive.push_back(curr);
                curr = 0;
                counting = false;
        }        
    }
    if (counting) consecutive.push_back(curr);

    return consecutive;
}

auto place_springs(std::string springs, uint32_t setup) -> std::string {
    for (short i = 0; i < 32 ; ++i) {
        uint32_t curr = (1 << i);
        if (curr > setup) break;
        if ((curr & setup)) {
            springs[springs.size() - 1 - i] = '#';

        }
    }
    return springs;
}

auto find_possible(std::string springs, const std::vector<short>& spring_data) -> uint64_t {
    uint64_t possible {0};
    const auto unknown_springs {get_unknown(springs)};
    std::replace(springs.begin(), springs.end(), '?', '.');

    uint32_t curr{0};
    std::vector<short> parsed;
    std::vector<uint32_t> tried;
    for (uint32_t i = 0; i <= unknown_springs; ++i) {
        curr = (i & unknown_springs);
        if (std::find(tried.begin(), tried.end(), curr) != tried.end()) continue;

        auto possibility = place_springs(springs, curr);
        parsed = get_consecutive(possibility);  
        if (parsed == spring_data) {
            ++possible;
        }
        tried.push_back(curr);
    }
    return possible;
}

auto part_1(std::ifstream& input) -> uint64_t {
    std::string line;
    uint64_t num_possible{0};
    uint32_t cnt {0};
    while (std::getline(input, line)) {
        auto boundry = line.find(' ');

        auto springs = line.substr(0, boundry);
        auto spring_data = tokenize(line.substr(boundry + 1)); 

        num_possible += find_possible(springs, spring_data);
        std::cout << ++cnt << std::endl;
    }

    return num_possible;
}

auto part_2(std::ifstream& input) -> uint64_t {
    return 0;
}


int main() {
    std::ifstream input("inputs/AoC_12.txt");
    std::cout << "Part 1: " << part_1(input) << std::endl;
    // std::cout << "Part 2: " << part_2(input) << std::endl;
}