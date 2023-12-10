#include <algorithm>
#include <fstream>
#include <iostream>
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
    std::string raw_times;
    std::string raw_records;
    std::vector<uint64_t> ways;

    std::getline(input, raw_times);
    std::getline(input, raw_records);
    auto times = get_tokens(split(raw_times, ':').second);
    auto records = get_tokens(split(raw_records, ':').second);


    for (uint64_t i = 0; i < times.size(); ++i) {
        uint64_t ways_curr {0};
        for (int64_t j = 0; j < std::stoll(times[i]); ++j) {
            if (j * (std::stoll(times[i]) - j) > std::stoll(records[i])) {
                ++ways_curr;
            }
        }
        ways.push_back(ways_curr);
    }    

    return std::reduce(ways.begin(), ways.end(), 1ull, std::multiplies<>());
}

auto part_2(std::ifstream& input) -> uint64_t {
    std::string raw_time;
    std::string raw_record;
    uint64_t ways {0};

    std::getline(input, raw_time);
    std::getline(input, raw_record);
    auto time = get_tokens(split(raw_time, ':').second);
    auto record = get_tokens(split(raw_record, ':').second);

    uint64_t time_num = std::stoll(std::accumulate(time.begin(), time.end(), std::string{}));
    uint64_t record_num = std::stoll(std::accumulate(record.begin(), record.end(), std::string{}));

    for (uint64_t i = 0; i < time_num; ++i) {
        if (i * (time_num - i) > record_num) {
            ++ways;
        }
    }    

    return ways;
}


int main() {
    std::ifstream input("inputs/AoC_06.txt");
    // std::cout << "Part 1: " << part_1(input) << std::endl;
    std::cout << "Part 2: " << part_2(input) << std::endl;
}