#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

auto tokenize(const std::string& data) -> std::vector<uint64_t> {
    std::vector<uint64_t> tokens;
    auto pos {data.find(',')};
    auto prev{0};
    while (pos != std::string::npos) {
        tokens.push_back(std::stoll(data.substr(prev, pos - prev)));
        prev = pos + 1;
        pos = data.find(',', prev);
    }
    tokens.push_back(std::stoll(data.substr(prev)));

    return tokens;
}

struct Solver {
private:
    std::vector<uint64_t> res;
public:
    std::string springs;
    std::vector<uint64_t> spring_data;
    std::vector<uint64_t> look_up;
    uint64_t springs_size;
    uint64_t spring_data_size;

    void setup() {
        spring_data_size = spring_data.size();
        springs += '.';
        springs_size = springs.size();

        look_up.reserve(springs_size - 1);
        for (uint64_t i = 0; i < springs_size - 1; ++i) {
            look_up[i] = springs.find('.', i + 1);
        }

        res.assign(spring_data_size * springs_size, ULLONG_MAX);
    }

    inline uint64_t get_from_res(uint64_t s_idx, uint64_t d_idx) const {
        return res[s_idx * spring_data_size + d_idx];
    }

    inline void set_in_res(uint64_t s_idx, uint64_t d_idx, uint64_t val) {
        res[s_idx * spring_data_size + d_idx] = val;
    }

    void print () {
        for(uint64_t i = 0; i < spring_data_size; ++i) {
            for(uint64_t j = 0; j < springs_size; ++j) {
                std::cout << get_from_res(i, j) << " ";
            }
            std::cout << std::endl;
        }
    }

    auto find_possible(uint64_t s_idx, uint64_t d_idx) -> uint64_t {
        if(s_idx >= springs_size) {
            return d_idx == spring_data_size;
        }
        if (d_idx == spring_data_size) {
            return springs.find('#', s_idx) == std::string::npos;
        }
        if (!(get_from_res(s_idx, d_idx) == ULLONG_MAX)) {
            return get_from_res(s_idx, d_idx);
        }
        switch(springs[s_idx]) {
            case '#': {
                auto num_cons = spring_data[d_idx];
                if (s_idx + num_cons <= look_up[s_idx] && springs[s_idx + num_cons] != '#') {
                    uint64_t tmp = find_possible(s_idx + num_cons + 1, d_idx + 1);
                    // std::cout << "1 loads " << tmp << std::endl;
                    set_in_res(s_idx, d_idx, tmp);
                    return tmp;
                }
                set_in_res(s_idx, d_idx, 0);
                return 0;
            }
            case '.':{
                uint64_t tmp = find_possible(s_idx + 1, d_idx);
                set_in_res(s_idx, d_idx, tmp);
                return tmp;
            }
            default:{
                uint64_t tmp {0};
                // #
                auto num_cons = spring_data[d_idx];
                if (look_up[s_idx] >= s_idx + num_cons && springs[s_idx + num_cons] != '#') {
                    tmp += find_possible(s_idx + num_cons + 1, d_idx + 1);
                }

                tmp += find_possible(s_idx + 1, d_idx);
                set_in_res(s_idx, d_idx, tmp);
                return tmp;
            }
        }
    }
};

auto part_1(std::ifstream& input) -> uint64_t {
    std::string line;
    uint64_t num_possible{0};
    while (std::getline(input, line)) {
        auto boundry = line.find(' ');
        Solver s;
        s.springs = line.substr(0, boundry);
        s.spring_data = tokenize(line.substr(boundry + 1)); 
        s.setup();
        num_possible += s.find_possible(0, 0);
    }

    return num_possible;
}

auto part_2(std::ifstream& input) -> uint64_t {
    std::string line;
    uint64_t num_possible{0};
    uint64_t cnt{0};
    while (std::getline(input, line)) {
        auto boundry = line.find(' ');
        Solver s;
        s.springs = line.substr(0, boundry);
        s.spring_data = tokenize(line.substr(boundry + 1)); 
        std::vector<uint64_t> spring_data_combined;
        s.springs += "?" + s.springs + "?" + s.springs + "?" + s.springs + "?" + s.springs;
        uint16_t spins{5};
        for (int i = 0; i < spins; ++i) {
            spring_data_combined.insert(spring_data_combined.end(), s.spring_data.begin(), s.spring_data.end());
        }
        s.spring_data = spring_data_combined;
        
        std::cout << std::endl;
        std::cout << ++cnt << std::endl;
        s.setup();
        num_possible += s.find_possible(0, 0);
    }

    return num_possible;}


int main() {
    std::ifstream input("inputs/AoC_12.txt");
    // std::cout << "Part 1: " << part_1(input) << std::endl;
    std::cout << "Part 2: " << part_2(input) << std::endl;
}