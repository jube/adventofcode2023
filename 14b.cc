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

int main() {

  std::vector<std::string> platform;

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.empty() || line == "\n") {
      continue;
    }

    platform.push_back(line);
  }

  std::size_t width = platform[0].size();
  std::size_t height = platform.size();

  constexpr int Cycles = 1'000'000'000;

  std::map<std::vector<std::string>, int> memory;

  for (int x = 0; x < Cycles; ++x) {
    if (auto iterator = memory.find(platform); iterator != memory.end()) {
      std::cout << "x = " << x << ", and before was: " << iterator->second << '\n';

      int length = x - iterator->second;
      std::cout << "Length: " << length << '\n';

      int cycles = (Cycles - x) / length;
      x += cycles * length;

      memory.clear();
    }

    std::cout << x << '\n';

    memory[platform] = x;

    // north
    for (std::size_t i = 0; i < width; ++i) {
      for (std::size_t j = 1; j < height; ++j) {
        if (platform[j][i] == 'O') {
          std::size_t k = j;

          while (k > 0 && platform[k - 1][i] == '.') {
            assert(platform[k][i] == 'O');
            std::swap(platform[k][i], platform[k - 1][i]);
            --k;
          }
        }
      }
    }

    // west
    for (std::size_t j = 0; j < height; ++j) {
      for (std::size_t i = 1; i < width; ++i) {
        if (platform[j][i] == 'O') {
          std::size_t k = i;

          while (k > 0 && platform[j][k - 1] == '.') {
            assert(platform[j][k] == 'O');
            std::swap(platform[j][k], platform[j][k - 1]);
            --k;
          }
        }
      }
    }

    // south
    for (std::size_t i = 0; i < width; ++i) {
      for (std::size_t j = height - 1; j-- > 0;) {
        if (platform[j][i] == 'O') {
          std::size_t k = j;

          while (k < height - 1 && platform[k + 1][i] == '.') {
            assert(platform[k][i] == 'O');
            std::swap(platform[k][i], platform[k + 1][i]);
            ++k;
          }
        }
      }
    }

    // east
    for (std::size_t j = 0; j < height; ++j) {
      for (std::size_t i = width - 1; i-- > 0;) {
        if (platform[j][i] == 'O') {
          std::size_t k = i;

          while (k < width - 1 && platform[j][k + 1] == '.') {
            assert(platform[j][k] == 'O');
            std::swap(platform[j][k], platform[j][k + 1]);
            ++k;
          }
        }
      }
    }

  }

  // for (auto& line : platform) {
  //   std::cout << line << '\n';
  // }
  // std::cout << '\n';

  std::size_t sum = 0;

  for (std::size_t j = 0; j < height; ++j) {
    sum += (height - j) * std::count(platform[j].begin(), platform[j].end(), 'O');
  }

  std::cout << "Sum: " << sum << '\n';
}
