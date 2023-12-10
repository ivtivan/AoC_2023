#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

auto split(std::string data, char sep = ' ') -> std::pair<std::string, std::string> {
    auto sep_idx = data.find(sep);
    return {data.substr(0, sep_idx), data.substr(sep_idx + 1)};
}

auto get_raw_rounds_data(std::string game_data) -> std::vector<std::string> {
    std::stringstream tokens(game_data);
    std::vector<std::string> rounds_data;
    std::string round_data;
    while (std::getline(tokens, round_data, ';')) {
        rounds_data.push_back(round_data);
    }
    return rounds_data;
}

auto get_parsed_rounds_data(std::string game_data) -> std::map<std::string, uint64_t> {
    auto raw_rounds_data = get_raw_rounds_data(game_data);
    std::map<std::string, uint64_t> num_per_color;
    for (const auto& round_data : raw_rounds_data) {
        std::stringstream tokens(round_data);
        std::string token;
        while (std::getline(tokens, token, ',')) {
            auto from = token.find_first_not_of(' ');
            auto [num, color] = split(token.substr(from));
            if (num_per_color.contains(color)) {
                num_per_color[color] = 
                    std::max(num_per_color[color], static_cast<uint64_t>(std::stoi(num)));
            }
            else {
                num_per_color[color] = std::stoi(num);
            }
        }
    }
    return num_per_color;
}

auto is_possible(std::map<std::string, uint64_t>& num_per_color) -> bool {
    return num_per_color["red"] <= 12 && num_per_color["green"] <= 13
        && num_per_color["blue"] <= 14;
}

auto part_1(std::ifstream& input) -> uint64_t {
    std::string game;
    uint64_t game_id_sum {0};

    while (std::getline(input, game)) {
        auto [game_id, game_data] = split(game, ':');
        auto num_per_color = get_parsed_rounds_data(game_data);
        if (is_possible(num_per_color)) {
            game_id_sum += std::stoi(split(game_id).second);
        }
    }

    return game_id_sum;
}

auto part_2(std::ifstream& input) -> uint64_t {
    std::string game;
    uint64_t power_sum {0};

    while (std::getline(input, game)) {
        auto [game_id, game_data] = split(game, ':');
        auto num_per_color = get_parsed_rounds_data(game_data);
            power_sum += num_per_color["red"] * num_per_color["green"] * num_per_color["blue"];
    }

    return power_sum;
}

int main() {
    std::ifstream input("inputs/AoC_02.txt");
    std::cout << "Part 1: " << part_1(input) << std::endl;
    //std::cout << "Part 2: " << part_2(input) << std::endl;
}