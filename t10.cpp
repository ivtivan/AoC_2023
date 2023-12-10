#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

//  why didn't I just make a Maze class? 

auto get_adjacent(const std::pair<uint, uint>& pos, const std::vector<std::string>& maze) -> std::string {
    const auto [row, col] = pos;
    switch (maze[row][col]) {
        case 'S': return "UDLR";
        case '-': return "LR";
        case '|': return "UD";
        case '7': return "DL";
        case 'J': return "UL";
        case 'L': return "UR";
        case 'F': return "DR";
        default: return "";
    }
    
}

auto has_up(const std::pair<uint, uint>& pos) -> bool {
    return pos.first > 0;
}

auto has_down(const std::pair<uint, uint>& pos, const std::vector<std::string>& maze) -> bool {
    return pos.first < maze.size() - 1;
}

auto has_left(const std::pair<uint, uint>& pos) -> bool {
    return pos.second > 0;
}

auto has_right(const std::pair<uint, uint>& pos, const std::vector<std::string>& maze) -> bool {
    return pos.second < maze[0].size() - 1;
}

auto filter_adjacent(std::string& adj, const std::pair<uint, uint>& pos, const std::vector<std::string>& maze) -> std::vector<std::pair<uint, uint>> {
    std::vector<std::pair<uint, uint>> filtered;
    const auto [row, col] = pos;
    for (int i = adj.size() - 1; i >= 0; --i) {
        switch(adj[i]) {
            case 'U': 
                if (has_up(pos)) {
                    if (std::string{"S|7F"}.find(maze[row - 1][col]) != std::string::npos) {
                        filtered.emplace_back(row - 1, col);
                    }
                }
                break;
            case 'D': 
                if (has_down(pos, maze)) {
                    if (std::string{"S|JL"}.find(maze[row + 1][col]) != std::string::npos) {
                        filtered.emplace_back(row +1, col);
                    }
                }
                break;
            case 'L':
                if (has_left(pos)) {
                    if (std::string{"S-LF"}.find(maze[row][col - 1]) != std::string::npos) {
                        filtered.emplace_back(row, col - 1);
                    }
                }
                break;
            case 'R':
                if (has_right(pos, maze)) {
                    if (std::string{"S-J7"}.find(maze[row][col + 1]) != std::string::npos) {
                        filtered.emplace_back(row, col + 1);
                    }
                }
                break;
            default: break;
        }
    }

    return filtered;
}


auto get_furthest(const std::pair<uint, uint>& start, const std::vector<std::string>& maze) -> uint64_t {
    const uint row_size {maze[0].size()}; // long enough
    std::vector<int> dist(maze.size() * row_size, -1);
    std::vector<std::pair<uint, uint>> curr{start};
    std::vector<std::pair<uint, uint>> next;
    int cnt {0};
    while (!curr.empty()) {
        for (auto el : curr) {
            dist[el.first * row_size + el.second] = cnt;
            std::string adj = get_adjacent(el, maze);
            auto filtered = filter_adjacent(adj, el, maze);
            for (auto f : filtered) {
                if (dist[f.first * row_size + f.second] == -1) {
                    next.push_back(std::move(f));
                }
            }
        }
        curr = std::move(next);
        next.clear();
        ++cnt;
    }

    return cnt - 1;
}

auto get_in(const std::pair<uint, uint>& start, const std::vector<std::string>& maze) -> uint64_t {
    const uint row_size {maze[0].size()}; // long enough
    std::vector<int> dist(maze.size() * row_size, -1);
    std::vector<std::pair<uint, uint>> curr{start};
    std::vector<std::pair<uint, uint>> next;
    int cnt {0};
    while (!curr.empty()) {
        for (auto el : curr) {
            dist[el.first * row_size + el.second] = cnt;
            std::string adj = get_adjacent(el, maze);
            auto filtered = filter_adjacent(adj, el, maze);
            for (auto f : filtered) {
                if (dist[f.first * row_size + f.second] == -1) {
                    next.push_back(std::move(f));
                }
            }
        }
        curr = std::move(next);
        next.clear();
    }

    uint64_t num {0};
    bool in{false};
    for (uint i = 0; i < maze.size(); ++i) {
        in = false;
        for (uint j = 0; j < maze[0].size(); ++j) {
            if (in && dist[i * row_size + j] == -1) {
                ++num;
            }
            if (dist[i * row_size + j] != -1 &&
                //  add S to the string if in the input it replaces |,7 or F
                std::string{"|7F"}.find(maze[i][j]) != std::string::npos) {
                    in = !in;
                }
        }
    }

    return num;
}


auto part_1(std::ifstream& input) -> uint64_t {
    std::string line;
    std::vector<std::string> maze;
    std::pair<uint, uint> start{0, 0};

    while (std::getline(input, line)) {
        if (auto pos = line.find('S'); pos != std::string::npos) {
            start.first = maze.size();
            start.second = pos;
        }
        maze.push_back(std::move(line));
    }
    return get_furthest(start, maze);
}

auto part_2(std::ifstream& input) -> uint64_t {
    std::string line;
    std::vector<std::string> maze;
    std::pair<uint, uint> start{0, 0};

    while (std::getline(input, line)) {
        if (auto pos = line.find('S'); pos != std::string::npos) {
            start.first = maze.size();
            start.second = pos;
        }
        maze.push_back(std::move(line));
    }
    return get_in(start, maze);
}


int main() {
    std::ifstream input("inputs/AoC_10.txt");
    // std::cout << "Part 1: " << part_1(input) << std::endl;
    std::cout << "Part 2: " << part_2(input) << std::endl;
}