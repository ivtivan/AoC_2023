#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

auto get_first_non_digit(const std::string& curr, uint16_t from) -> uint16_t {
    while (from < curr.size() && std::isdigit(curr[from])) {
        from++;
    }
    return from;
}

auto is_char(char c) -> bool {
    return c != '.' && !std::isdigit(c);
}

auto has_char_in(const std::string& line, uint16_t from, uint16_t to) -> bool {
    if (line == "") return false;

    for (; from < to; ++from) {
        if (is_char(line[from])) return true;
    }
    return false;
}

auto get_number_boundries(const std::string& curr) -> std::vector<std::pair<uint16_t, uint16_t>> {
    std::vector<std::pair<uint16_t, uint16_t>> boundries;

    std::size_t pos {0};
    const std::string digits {"0123456789"};
    std::size_t from;
    while ((from = curr.find_first_of(digits, pos)) != std::string::npos) {
        uint16_t to = get_first_non_digit(curr, from);
        boundries.emplace_back(from, to);
        pos = to;
    }

    return boundries;
}

auto get_gears(const std::string& curr) -> std::vector<uint16_t> {
    std::vector<uint16_t> gears;

    std::size_t pos {0};
    std::size_t idx;
    while ((idx = curr.find('*', pos)) != std::string::npos) {
        gears.push_back(idx);
        pos = idx + 1;
    }

    return gears;
}

auto nums_in(const std::string& line, uint16_t from, uint16_t to) -> uint8_t {
    if (line == "") return false;
    uint8_t nums{0};
    bool is_num = false;
    for (; from <= to; ++from) {
        if (std::isdigit(line[from]))  {
            if (!is_num) {
                is_num = true;
                ++nums;
            }
        }
        else {
            is_num = false;
        }
    }
    return nums;
}

auto prod_in(const std::string& line, uint16_t from, uint16_t to) -> uint64_t {
    uint64_t prod{1};
    auto nums = get_number_boundries(line);

    std::cout << from << " " << to << std::endl; 
    for (auto [n_from, n_to] : nums) {
        if ((from <= n_from && n_from <= to) ||
            (from <= n_to - 1 && n_to - 1 <= to)) {
                prod *= std::stoi(line.substr(n_from, n_to - n_from));
            }
    }
    return prod;
}

auto part_1(std::ifstream& input) -> uint64_t {
    uint64_t sum {0};

    std::string prev{""};
    std::string curr{""};
    std::string next{""};
    input >> next;

    while (next != "") {
        prev = std::move(curr);
        curr = std::move(next);   
        if (!(input >> next)) next = "";

        auto num_boundries = get_number_boundries(curr);

        for (auto [from, to] : num_boundries) {
            uint16_t v_from = from - 1;
            uint16_t v_to = to;
            if (from == 0) v_from = from;
            if (v_to == curr.size()) --v_to; 
            if (is_char(curr[v_from]) || is_char(curr[v_to]) ||
                    has_char_in(prev, v_from, v_to + 1) || has_char_in(next, v_from, v_to + 1)) {
                sum += std::stoi(curr.substr(from, to - from));
            }
        }
    }

    return sum;
}

auto part_2(std::ifstream& input) -> uint64_t {
    uint64_t sum {0};

    std::string prev{""};
    std::string curr{""};
    std::string next{""};
    input >> next;

    while (next != "") {
        prev = std::move(curr);
        curr = std::move(next);   
        if (!(input >> next)) next = "";

        auto gears = get_gears(curr);
    std::cout << gears.size() << std::endl;
        for (auto gear : gears) {
            uint16_t v_from = gear - 1;
            uint16_t v_to = gear + 1;
            if (gear == 0) v_from = gear;
            if (v_to == curr.size()) --v_to; 

            uint8_t num_adjacent = std::isdigit(curr[v_from]) + std::isdigit(curr[v_to]) 
                + nums_in(prev, v_from, v_to) + nums_in(next, v_from, v_to);
            
            if (num_adjacent != 2) continue;
        
            sum += prod_in(curr, v_from, v_to) *
                prod_in(prev, v_from, v_to) * prod_in(next, v_from, v_to);

        }
    }

    return sum;
}

int main() {
    std::ifstream input("inputs/AoC_03.txt");
    // std::cout << "Part 1: " << part_1(input) << std::endl;
    std::cout << "Part 2: " << part_2(input) << std::endl;
}