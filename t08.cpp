#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

auto split(const std::string& data, char sep = ' ') -> std::pair<std::string, std::string> {
    auto sep_idx = data.find(sep);
    return {data.substr(0, sep_idx), data.substr(sep_idx + 1)};
}

auto get_tokens(const std::string& tokens_raw) -> std::vector<std::string> {
    std::stringstream tokens_src(tokens_raw);
    std::vector<std::string> tokens;
    std::string token;
    while (tokens_src >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

auto parse_to(const std::string& to_raw) -> std::pair<std::string, std::string> {
    auto begin = to_raw.find('(') + 1;
    auto to = to_raw.find(')');
    auto [L_parsed, R_parsed] = split(to_raw.substr(begin, to - begin), ',');
    R_parsed = R_parsed.substr(1);
    return {L_parsed, R_parsed};
}

auto get_time_needed(const std::string& directions, std::map<std::string, std::pair<std::string, std::string>> mappings) -> uint64_t {
    uint64_t needed {0};
    std::string curr = "AAA";
    std::string target = "ZZZ";
    
    while (curr != target) {
        if (directions[needed % directions.size()] == 'L') {
            curr = mappings[curr].first; 
        }
        else {
            curr = mappings[curr].second;
        }
        ++needed;
    }

    return needed;
}

auto get_time_needed_ghost(const std::string& directions, std::map<std::string, std::pair<std::string, std::string>> mappings) -> uint64_t {
    std::vector<std::string> beginnings;
    for (auto& el : mappings) {
        if (el.first.back() == 'A') {
            beginnings.push_back(el.first);
        }
    }

    std::vector<uint64_t> all_needed;
    
    for (auto& curr : beginnings) {
        uint64_t needed {0};
        
        while (curr.back() != 'Z') {
            if (directions[needed % directions.size()] == 'L') {
                curr = mappings[curr].first; 
            }
            else {
                curr = mappings[curr].second;
            }
            ++needed;
        }

        all_needed.push_back(needed);
    }

    return std::reduce(all_needed.begin(), all_needed.end(), 1ull, std::lcm<uint64_t, uint64_t>);
}


auto part_1(std::ifstream& input) -> uint64_t {
    std::string directions;
    std::string mapping;
    std::map<std::string, std::pair<std::string, std::string>> mappings;

    std::getline(input, directions);
    std::getline(input, mapping);

    // delete empty line from input
    while (std::getline(input, mapping)) {
        auto [from, to_raw] = split(mapping, '=');
        from = from.substr(0, from.size() - 1);
        mappings[from] = parse_to(to_raw);
    }

    return get_time_needed(directions, mappings);
}

auto part_2(std::ifstream& input) -> uint64_t {
    std::string directions;
    std::string mapping;
    std::map<std::string, std::pair<std::string, std::string>> mappings;

    std::getline(input, directions);
    std::getline(input, mapping);

    // delete empty line from input
    while (std::getline(input, mapping)) {
        auto [from, to_raw] = split(mapping, '=');
        from = from.substr(0, from.size() - 1);
        mappings[from] = parse_to(to_raw);
    }

    return get_time_needed_ghost(directions, mappings);
}


int main() {
    std::ifstream input("inputs/AoC_08.txt");
    // std::cout << "Part 1: " << part_1(input) << std::endl;
    std::cout << "Part 2: " << part_2(input) << std::endl;
}