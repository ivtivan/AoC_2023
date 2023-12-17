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


auto push_north(std::vector<std::string>& rocks) -> void {
    for (uint64_t col {0}; col < rocks[0].size(); ++col) {
        uint64_t write {0};
        for (uint64_t row {0}; row < rocks.size(); ++row) {
            if (rocks[row][col] == '#') {
                write = row + 1;
            }
            else if (rocks[row][col] == 'O') {
                rocks[row][col] = '.';
                rocks[write][col] = 'O';
                ++write;
            }
        }
    }
}

auto push_south(std::vector<std::string>& rocks) -> void {
    for (uint64_t col {0}; col < rocks[0].size(); ++col) {
        uint64_t write {rocks.size() - 1};
        for (int64_t row {rocks.size() - 1}; row >= 0; --row) {
            if (rocks[row][col] == '#') {
                write = row - 1;
            }
            else if (rocks[row][col] == 'O') {
                rocks[row][col] = '.';
                rocks[write][col] = 'O';
                --write;
            }
        }
    }
}

auto push_west(std::vector<std::string>& rocks) -> void {
    for (uint64_t row {0}; row < rocks.size(); ++row) {
        uint64_t write {0};
        for (uint64_t col {0}; col < rocks[0].size(); ++col) {
            if (rocks[row][col] == '#') {
                write = col + 1;
            }
            else if (rocks[row][col] == 'O') {
                rocks[row][col] = '.';
                rocks[row][write] = 'O';
                ++write;
            }
        }
    }
}

auto push_east(std::vector<std::string>& rocks) -> void {
    for (uint64_t row {0}; row < rocks.size(); ++row) {
        uint64_t write {rocks[0].size() - 1};
        for (int64_t col {rocks[0].size() - 1}; col >= 0; --col) {
            if (rocks[row][col] == '#') {
                write = col - 1;
            }
            else if (rocks[row][col] == 'O') {
                rocks[row][col] = '.';
                rocks[row][write] = 'O';
                --write;
            }
        }
    }
}

auto get_north_load(const std::vector<std::string>& rocks) -> uint64_t {
    uint64_t load {0};
    uint64_t rows = rocks.size();
    for (uint64_t i{0}; i < rows; ++i) {
        for (const auto el : rocks[i]) {
            if (el == 'O') load += rows - i;
        }
    }
    return load;
}

auto part_1(std::ifstream& input) -> uint64_t {
    std::string line{};
    std::vector<std::string> rocks;
    while (std::getline(input, line)) {
        rocks.push_back(line);
    }

    push_north(rocks);

    return get_north_load(rocks);
}

auto part_2(std::ifstream& input) -> uint64_t {
    std::string line{};
    std::vector<std::string> rocks;
    std::vector<std::vector<std::string>> history;
    while (std::getline(input, line)) {
        rocks.push_back(line);
    }
    uint64_t cycles {1000000000};

    for (uint64_t i {0}; i < cycles; ++i) {
        push_north(rocks);
        push_west(rocks);
        push_south(rocks);
        push_east(rocks);
        auto curr_load = get_north_load(rocks);
        auto found = std::find(history.begin(), history.end(), rocks);
        if (found != history.end()) {
            auto from = std::distance(history.begin(), found);
            std::cout << "pushed " << curr_load << std::endl;
            std::cout << "loop from " << from << " to " << i - 1 << std::endl;
            // return 0;
            auto index = from + (cycles - from - 1) % (i - from);
            std::cout << "Resulst same as at " << index << std::endl;
            return get_north_load(history[index]);
        }
        history.push_back(rocks);
        std::cout << "pushed " << curr_load << std::endl;
    }

    return get_north_load(rocks);
}


int main() {
    std::ifstream input("inputs/AoC_14.txt");
    // std::cout << "Part 1: " << part_1(input) << std::endl;
    std::cout << "Part 2: " << part_2(input) << std::endl;
}