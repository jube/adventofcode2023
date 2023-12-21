#include <cassert>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <tuple>
#include <vector>

void step(const std::vector<std::string>& input, std::vector<std::string>& output) {
  std::size_t width = input[0].size();
  std::size_t height = input.size();

  for (std::size_t y = 0; y < height; ++y) {
    for (std::size_t x = 0; x < width; ++x) {
      char c = input[y][x];

      if (c == '#') {
        continue;
      }

      if (c == 'O' || c == 'S') {
        if (x > 0 && input[y][x - 1] != '#') {
          output[y][x - 1] = 'O';
        }

        if (x < width - 1 && input[y][x + 1] != '#') {
          output[y][x + 1] = 'O';
        }

        if (y > 0 && input[y - 1][x] != '#') {
          output[y - 1][x] = 'O';
        }

        if (y < height - 1 && input[y + 1][x] != '#') {
          output[y + 1][x] = 'O';
        }
      }
    }
  }

}

int main() {

  std::vector<std::string> ground;

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.empty() || line == "\n") {
      continue;
    }

    ground.push_back(line);
  }

  auto empty = ground;

  for (auto& line : empty) {
    for (auto& c : line) {
      if (c == 'S') {
        c = '.';
        break;
      }
    }
  }

  for (int i = 0; i < 64; ++i) {
    auto next = empty;
    step(ground, next);
    ground = next;
  }

  int count = 0;

  for (auto& line : ground) {
    for (auto& c : line) {
      std::cout << c;

      if (c == 'O') {
        ++count;
      }
    }
    std::cout << '\n';
  }

  std::cout << "Count: " << count << '\n';
}
