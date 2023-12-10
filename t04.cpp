#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

auto split(const std::string& data, char sep = ' ') -> std::pair<std::string, std::string> {
    auto sep_idx = data.find(sep);
    return {data.substr(0, sep_idx), data.substr(sep_idx + 1)};
}

auto get_cards(const std::string& nums_raw) -> std::vector<std::string> {
    std::stringstream tokens(nums_raw);
    std::vector<std::string> nums;
    std::string num;
    while (std::getline(tokens, num, ' ')) {
        if (num != "") nums.push_back(num);
    }
    return nums;
}

auto get_points(std::vector<std::string> els, std::vector<std::string> search_space) -> uint64_t {
    uint64_t points {0};
    for (const auto& el : els) {
        if (std::find(search_space.begin(), search_space.end(), el) != search_space.end()) {
            if (points) points *= 2;
            else points = 1;
        }
    }

    return points;
}

auto get_num_winning(std::vector<std::string> els, std::vector<std::string> search_space) -> uint64_t {
    uint64_t num {0};
    for (const auto& el : els) {
        if (std::find(search_space.begin(), search_space.end(), el) != search_space.end()) {
            num += 1;
        }
    }

    return num;
}


auto part_1(std::ifstream& input) -> uint64_t {
    std::string card;
    uint64_t points {0};

    while (std::getline(input, card)) {
        auto [card_id, card_data] = split(card, ':');
        auto [winning_raw, chosen_raw] = split(card_data, '|');
        auto winning_parsed = get_cards(winning_raw);
        auto chosen_parsed = get_cards(chosen_raw);
        
        points += get_points(chosen_parsed, winning_parsed);
    }

    return points;
}

auto part_2(std::ifstream& input) -> uint64_t {
    std::string card;
    uint64_t num_cards {0};
    std::vector<uint64_t> next_copies;
    while (std::getline(input, card)) {
        ++num_cards;
        auto [card_id, card_data] = split(card, ':');
        auto [winning_raw, chosen_raw] = split(card_data, '|');
        auto winning_parsed = get_cards(winning_raw);
        auto chosen_parsed = get_cards(chosen_raw);
        
        auto won {get_num_winning(chosen_parsed, winning_parsed)};
        auto num_winning {1ull};
        if(next_copies.size()) {
            num_winning += next_copies.front() * num_winning;
            num_cards += next_copies.front();
            next_copies.erase(next_copies.begin());
        }
        for (auto i {0ull}; i < std::min(next_copies.size(), won); ++i) {
            next_copies[i] += num_winning;
        }
        for (auto i {next_copies.size()}; i < won; ++i) {
            next_copies.push_back(num_winning);
        }
    }

    return num_cards;
}

int main() {
    std::ifstream input("inputs/AoC_04.txt");
    // std::cout << "Part 1: " << part_1(input) << std::endl;
    std::cout << "Part 2: " << part_2(input) << std::endl;
}