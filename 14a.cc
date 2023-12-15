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

  for (std::size_t i = 0; i < width; ++i) {
    for (std::size_t j = 0; j < height; ++j) {
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

  for (auto& line : platform) {
    std::cout << line << '\n';
  }

  std::size_t sum = 0;

  for (std::size_t j = 0; j < height; ++j) {
    sum += (height - j) * std::count(platform[j].begin(), platform[j].end(), 'O');
  }

  std::cout << "Sum: " << sum << '\n';
}
