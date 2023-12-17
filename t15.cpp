#include <algorithm>
#include <array>
#include <climits>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

auto compute_hash(const std::string& data) -> uint64_t {
    uint64_t initial {0};
    for (const auto el : data) {
        initial += el;
        initial = (17 * initial) % 256;
    }

    return initial;
}

class Lens {
public:
    std::string label;
    uint64_t val;

#ifdef DEBUG
    friend std::ostream& operator<< (std::ostream& os, const Lens& lens) {
        os << "[" << lens.label << " " << lens.val << "]";
        return os;
    }
#endif
};

class Hashmap {
private:
    std::array<std::vector<Lens>, 256> boxes;
public:
    void insert(const std::string& label, const uint64_t& val) {
        const auto hash = compute_hash(label);
        for (auto& el : boxes[hash]) {
            if (el.label == label) {
                el.val = val;
                return;
            }
        }
        boxes[hash].emplace_back(label, val);
    }

    void erase(const std::string& label) {
        const auto hash = compute_hash(label);
        for (auto it = boxes[hash].cbegin(); it != boxes[hash].cend(); ++it) {
            if (it->label == label) {
                boxes[hash].erase(it);
                return;
            }
        }
    }

    auto power() -> uint64_t {
        uint64_t power_ {0};
        for (uint64_t i = 0; i < boxes.size(); ++i) {
            for (uint64_t j = 0; j < boxes[i].size(); ++j) {
                uint64_t power_inc {i + 1};
                power_inc *= (j + 1) * boxes[i][j].val;
#ifdef DEBUG
                std::cout << "Power inc for " << boxes[i][j] << ": " << power_inc << std::endl;
#endif
                power_ += power_inc;
            }
        }
        return power_;
    }

#ifdef DEBUG
    friend std::ostream& operator<< (std::ostream& os, const Hashmap& hm) {
        for (uint64_t i = 0; i < hm.boxes.size(); ++i) {
            if (!hm.boxes[i].empty()) {
                os << "Box " << i + 1 << ": ";
                for (const auto& lens : hm.boxes[i]) {
                    os << lens << " ";
                }
                os << std::endl;
            }
        }
        return os;
    }
#endif
};

void execute(const std::string& command, Hashmap& hm) {
    if (auto pos = command.find('='); pos != std::string::npos) {
        hm.insert(command.substr(0, pos), std::stoll(command.substr(pos + 1)));
    }
    else {
        hm.erase(command.substr(0, command.find('-')));
    }
}

auto part_1(std::ifstream& input) -> uint64_t {
    std::string read{};
    uint64_t hash_val {0};
    uint64_t sum_hash_val {0};
    while (std::getline(input, read, ',')) {
        hash_val = compute_hash(read);
#ifdef DEBUG
        std::cout << "The hash value of " << read << " is " << hash_val << std::endl;
#endif    
        sum_hash_val += hash_val;
    }

    return sum_hash_val;
}

auto part_2(std::ifstream& input) -> uint64_t {
    std::string read{};
    Hashmap hm;
    while (std::getline(input, read, ',')) {
        execute(read, hm);
    }

    return hm.power();
}

int main() {
    std::ifstream input("inputs/AoC_15.txt");
    // std::cout << "Part 1: " << part_1(input) << std::endl;
    std::cout << "Part 2: " << part_2(input) << std::endl;
}