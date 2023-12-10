#include <fstream>
#include <iostream>
#include <string>
#include <array>

int main() {
    std::ifstream input("input.txt");
    std::string line;
    uint64_t sum {0};
    std::string digits = "0123456789";
    std::array<std::pair<std::string, uint64_t>, 10> spelled_digits {
        std::pair<std::string, uint64_t>{"zero", 0},
        std::pair<std::string, uint64_t>{"one", 1},
        std::pair<std::string, uint64_t>{"two", 2},
        std::pair<std::string, uint64_t>{"three", 3},
        std::pair<std::string, uint64_t>{"four", 4},
        std::pair<std::string, uint64_t>{"five", 5},
        std::pair<std::string, uint64_t>{"six", 6},
        std::pair<std::string, uint64_t>{"seven", 7},
        std::pair<std::string, uint64_t>{"eight", 8},
        std::pair<std::string, uint64_t>{"nine", 9}
    };
    while (input >> line) {
        auto first = line.find_first_of(digits);
        auto last = line.find_last_of(digits);
        int tens = line[first] - '0';
        int singles = line[last] - '0';
        
        for (const auto& dig : spelled_digits) {
            auto from {line.find(dig.first)};

            while (from != std::string::npos) {
                if (from < first) {
                    first = from;
                    tens = dig.second;
                }
                if (from > last) {
                    last = from;
                    singles = dig.second;
                }
                from = line.find(dig.first, from + 1);
            }
        }

        sum += 10 * tens + singles;
    }
    std::cout << sum << std::endl;
}