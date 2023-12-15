#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

auto get_vertical(const std::vector<std::string>& line, const uint16_t delta = 0) -> uint16_t {
    uint16_t right{0};
    for (uint16_t left = 1; left < line[0].size(); ++left) {
        uint16_t curr_delta{0};
        bool is_ok {true};
        right = line[0].size() - left;
        for (uint16_t i = 0; i < line.size(); ++i) {
            for (int j = 0; j < std::min(left, right); ++j) {
                if(line[i][left - j - 1] != line[i][left + j]) {
                    curr_delta++;
                    if (curr_delta > delta) {
                        is_ok = false;
                        break;
                    }
                }
            }
        }
        if (is_ok && delta == curr_delta) {
            return left;
        }
    }

    return 0;
}

auto get_horizontal(const std::vector<std::string>& lines, const uint16_t delta = 0) -> uint16_t {
    uint16_t down{0};
    for (uint16_t up = 1; up < lines.size(); ++up) {
        uint16_t curr_delta{0};
        bool is_ok {true};
        down = lines.size() - up;
        for (uint16_t i = 0; i < lines[0].size(); ++i) {
            for (int j = 0; j < std::min(up, down); ++j) {
                if(lines[up - j - 1][i] != lines[up + j][i]) {
                    curr_delta++;
                    if (curr_delta > delta) {
                        is_ok = false;
                        break;
                    }
                }
            }
        }
        if (is_ok && delta == curr_delta) {
            return up;
        }
    }

    return 0;
}

auto part_1(std::ifstream& input) -> uint64_t {
    std::string line{};
    std::vector<std::string> lines;
    bool is_new {true};
    uint64_t res{0};
    while (std::getline(input, line)) {
        if (line == "") {
            res += get_vertical(lines);
            res += 100 * get_horizontal(lines);
            is_new = true;
            lines.clear();
            continue;
        }
        if (is_new) {
            is_new = false;
        }
        lines.push_back(line);
    }
    res += get_vertical(lines);
    res += 100 * get_horizontal(lines);

    return res;
}

auto part_2(std::ifstream& input) -> uint64_t {
    std::string line{};
    std::vector<std::string> lines;
    bool is_new {true};
    uint64_t res{0};
    while (std::getline(input, line)) {
        if (line == "") {
            res += get_vertical(lines, 1);
            res += 100 * get_horizontal(lines, 1);
            is_new = true;
            lines.clear();
            continue;
        }
        if (is_new) {
            is_new = false;
        }
        lines.push_back(line);
    }
    res += get_vertical(lines, 1);
    res += 100 * get_horizontal(lines, 1);

    return res;
}


int main() {
    std::ifstream input("inputs/AoC_13.txt");
    // std::cout << "Part 1: " << part_1(input) << std::endl;
    std::cout << "Part 2: " << part_2(input) << std::endl;
}