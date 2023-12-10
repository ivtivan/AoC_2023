#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using translation = std::tuple<int64_t, int64_t, int64_t>;

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

auto to_int64_all(const std::vector<std::string>& data) -> std::vector<int64_t> {
    std::vector<int64_t> ints(data.size());
    std::transform(data.begin(), data.end(), ints.begin(), [](std::string num) { return std::stoll(num);});
    return ints;
}

auto to_int64_three(const std::vector<std::string>& data) -> translation {
    return {std::stoll(data.at(0)), std::stoll(data.at(1)), std::stoll(data.at(2))};
}

auto translate_seeds(const std::vector<int64_t>& seeds, const std::vector<std::vector<translation>>& translations)
    -> std::vector<int64_t> {

    std::vector<int64_t> curr = seeds;
    for (const auto& translation_table : translations) {
        for (int i = curr.size() - 1; i >= 0; --i) {
            auto lower = std::lower_bound(translation_table.begin(), translation_table.end(), translation{0, curr[i], 0},
                [&](const translation& a, const translation& b) {
                    return get<1>(a) <= get<1>(b);
            });
            if (lower == translation_table.begin()) continue;
            lower = std::prev(lower);
            if (std::get<1>(*lower) <= curr[i] && get<1>(*lower) + get<2>(*lower) > curr[i]) {
                curr[i] = curr[i] - get<1>(*lower) + get<0>(*lower);
            }
        }
    }
    
    return curr;
}

auto get_tranlation_tables(std::ifstream& input) -> std::vector<std::vector<translation>> {
    std::vector<std::vector<translation>> translation_tables;
    std::vector<translation> curr_table;
    
    std::string row;
    while (std::getline(input, row)) {
        if (std::find(row.cbegin(), row.cend(), ':') != row.end() || row.empty()) {
            if(!curr_table.empty()) {
                std::sort(curr_table.begin(), curr_table.end(), [](translation a, translation b) {
                    return get<1>(a) < get<1>(b);
                });
                translation_tables.push_back(std::move(curr_table));
                curr_table.clear();
            }
            continue;
        }
        curr_table.push_back(to_int64_three(get_tokens(row)));
    }
    std::sort(curr_table.begin(), curr_table.end(), [](translation a, translation b) {
        return get<1>(a) < get<1>(b);
    });
    translation_tables.push_back(std::move(curr_table));

    return translation_tables;
}

auto part_1(std::ifstream& input) -> uint64_t {
    std::string raw_seeds;

    std::getline(input, raw_seeds);
    auto seeds = to_int64_all(get_tokens(split(raw_seeds, ':').second));

    auto translation_tables = get_tranlation_tables(input);

    auto results = translate_seeds(seeds, translation_tables);
    return *std::min_element(results.begin(), results.end());
}

auto part_2(std::ifstream& input) -> uint64_t {
    std::string raw_seeds;

    std::getline(input, raw_seeds);
    auto seeds = to_int64_all(get_tokens(split(raw_seeds, ':').second));

    auto translation_tables = get_tranlation_tables(input);


    std::vector<int64_t> min_from_group;
    for (uint64_t i = 0; i < seeds.size(); i += 2) {
        std::vector<int64_t> group(seeds[i + 1]);
        std::iota(group.begin(), group.end(), seeds[i]);
        auto results = translate_seeds(group, translation_tables);
        min_from_group.push_back(*std::min_element(results.begin(), results.end()));
        std::cout << min_from_group.back() << std::endl;
    }

    return *std::min_element(min_from_group.begin(), min_from_group.end());
}


int main() {
    std::ifstream input("inputs/AoC_05.txt");
    // std::cout << "Part 1: " << part_1(input) << std::endl;
    std::cout << "Part 2: " << part_2(input) << std::endl;
}