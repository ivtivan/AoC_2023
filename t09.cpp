#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

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

auto get_folds_until_zeros(const std::vector<int64_t>& data) -> std::vector<std::vector<int64_t>> {
    std::vector<std::vector<int64_t>> all_rows {data};
    
    while(!std::all_of(all_rows.back().begin(), all_rows.back().end(), [](int64_t el) { return el == 0; })) {
        std::vector<int64_t> diffed(all_rows.back().size() - 1);
        std::transform(std::next(all_rows.back().begin()), all_rows.back().end(), all_rows.back().begin(), diffed.begin(), 
            [](auto a, auto b) { return a - b;});
        all_rows.push_back(diffed);
    }

    return all_rows;
}

auto get_next(const std::vector<int64_t>& data) -> int64_t {
    auto folded = get_folds_until_zeros(data);
    
    int64_t prev_last{0};
    for(auto i = std::next(folded.rbegin()); i != folded.rend(); ++i) {
        prev_last = i->back() + prev_last;
    }

    return prev_last;
}

auto get_prev(const std::vector<int64_t>& data) -> int64_t {
    auto folded = get_folds_until_zeros(data);
    
    int64_t prev_first{0};
    for(auto i = std::next(folded.rbegin()); i != folded.rend(); ++i) {
        prev_first = i->front() - prev_first;
    }

    return prev_first;
}

auto part_1(std::ifstream& input) -> uint64_t {
    std::string data;
    int64_t sum {0};

    while (std::getline(input, data)) {
        auto values = to_int64_all(get_tokens(data));
        sum += get_next(values);
    }

    return sum;
}

auto part_2(std::ifstream& input) -> uint64_t {
    std::string directions;
    std::string data;
    int64_t sum {0};

    input >> directions;
    input >> data;

    while (std::getline(input, data)) {
        auto values = to_int64_all(get_tokens(data));
        sum += get_prev(values);
    }

    return sum;
}


int main() {
    std::ifstream input("inputs/AoC_09.txt");
    // std::cout << "Part 1: " << part_1(input) << std::endl;
    std::cout << "Part 2: " << part_2(input) << std::endl;
}