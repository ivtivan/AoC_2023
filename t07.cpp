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

struct greater_hands_raw {
    bool operator()(const std::string& lhs, const std::string& rhs) {
        std::vector<char> cards = {'A', 'K', 'Q', 'J', 'T'};
        for (char i = '9'; i >= '2'; --i) {
            cards.push_back(i);
        }
        uint i {0};
        while(lhs[i] == rhs[i] && i < lhs.size()) ++i;
        if (i == lhs.size()) return false;
        return std::find(cards.begin(), cards.end(), lhs[i]) < 
            std::find(cards.begin(), cards.end(), rhs[i]);
    }
};

struct greater_hands_raw_mod {
    bool operator()(const std::string& lhs, const std::string& rhs) {
        std::vector<char> cards = {'A', 'K', 'Q', 'T'};
        for (char i = '9'; i >= '2'; --i) {
            cards.push_back(i);
        }
        cards.push_back('J');
        uint i {0};
        while(lhs[i] == rhs[i] && i < lhs.size()) ++i;
        if (i == lhs.size()) return false;
        return std::find(cards.begin(), cards.end(), lhs[i]) < 
            std::find(cards.begin(), cards.end(), rhs[i]);
    }
};

auto parse_hand_data(const std::string& hand) -> std::vector<std::pair<char, short>> {
    std::vector<std::pair<char, short>> parsed;
    parsed.reserve(hand.size());
    for (const char& el : hand) {
        auto found = std::find_if(parsed.begin(), parsed.end(), [&el](auto a) {return a.first == el;}); 
        if (found != parsed.end()) {
            found->second++;
        }
        else {
            parsed.emplace_back(el, 1);
        }
    }
    std::sort(parsed.begin(), parsed.end(), [](auto a, auto b) {
        return a.second > b.second;
    });
    return parsed;
}

auto parse_hand_data_mod(const std::string& hand) -> std::vector<std::pair<char, short>> {
    std::vector<std::pair<char, short>> parsed;
    parsed.reserve(hand.size());
    for (const char& el : hand) {
        auto found = std::find_if(parsed.begin(), parsed.end(), [&el](auto a) {return a.first == el;}); 
        if (found != parsed.end()) {
            found->second++;
        }
        else {
            parsed.emplace_back(el, 1);
        }
    }
    std::sort(parsed.begin(), parsed.end(), [](auto a, auto b) {
        return a.second > b.second;
    });

    auto joker = std::find_if(parsed.begin(), parsed.end(), [](auto a) {return a.first == 'J';});
    if (joker != parsed.end() && parsed.size() != 1) {
        auto holder  = joker->second;
        parsed.erase(joker);
        parsed.begin()->second += holder;
    }


    return parsed;
}

struct greater_hands {
    bool operator()(const std::string& lhs, const std::string& rhs) const {
        auto lhs_parsed = parse_hand_data(lhs);
        auto rhs_parsed = parse_hand_data(rhs);

        if (lhs_parsed.begin()->second != rhs_parsed.begin()->second) {
            return lhs_parsed.begin()->second > rhs_parsed.begin()->second;
        }
        if (lhs_parsed.begin()->second > 3 || lhs_parsed.begin()->second == 1) {
            return greater_hands_raw{}(lhs, rhs);
        }
        if (std::next(lhs_parsed.begin())->second != std::next(rhs_parsed.begin())->second) {
            return std::next(lhs_parsed.begin())->second > std::next(rhs_parsed.begin())->second;
        }
        return greater_hands_raw{}(lhs, rhs);
    }
};

struct greater_hands_mod {
    bool operator()(const std::string& lhs, const std::string& rhs) const {
        auto lhs_parsed = parse_hand_data_mod(lhs);
        auto rhs_parsed = parse_hand_data_mod(rhs);

        if (lhs_parsed.begin()->second != rhs_parsed.begin()->second) {
            return lhs_parsed.begin()->second > rhs_parsed.begin()->second;
        }
        if (lhs_parsed.begin()->second > 3 || lhs_parsed.begin()->second == 1) {
            return greater_hands_raw_mod{}(lhs, rhs);
        }
        if (std::next(lhs_parsed.begin())->second != std::next(rhs_parsed.begin())->second) {
            return std::next(lhs_parsed.begin())->second > std::next(rhs_parsed.begin())->second;
        }
        return greater_hands_raw_mod{}(lhs, rhs);
    }
};


auto part_1(std::ifstream& input) -> uint64_t {
    std::string hand_data;
    std::map<std::string, uint64_t, greater_hands> hands_bids;
    uint64_t res{0};

    while(std::getline(input, hand_data)) {
        auto [hand, raw_bid] = split(hand_data);
        hands_bids[hand] = std::stoll(raw_bid);
    }

    uint64_t cnt {hands_bids.size()};
    for (auto el : hands_bids) {
        res += el.second * cnt;
        cnt--;
    }

    return res;
}

auto part_2(std::ifstream& input) -> uint64_t {
    std::string hand_data;
    std::map<std::string, uint64_t, greater_hands_mod> hands_bids;
    uint64_t res{0};

    while(std::getline(input, hand_data)) {
        auto [hand, raw_bid] = split(hand_data);
        hands_bids[hand] = std::stoll(raw_bid);
    }

    uint64_t cnt {hands_bids.size()};
    for (auto el : hands_bids) {
        res += el.second * cnt;
        cnt--;
    }

    return res;
}


int main() {
    std::ifstream input("inputs/AoC_07.txt");
    // std::cout << "Part 1: " << part_1(input) << std::endl;
    std::cout << "Part 2: " << part_2(input) << std::endl;
}