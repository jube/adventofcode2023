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
  std::string lr;
  std::getline(std::cin, lr);

  std::map<std::string, std::pair<std::string, std::string>> instructions;

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.empty() || line == "\n") {
      continue;
    }

    assert(line.size() == 16);
    std::string origin = line.substr(0, 3);
    std::string left = line.substr(7, 3);
    std::string right = line.substr(12, 3);

    instructions[origin] = std::make_pair(left, right);
  }

  std::vector<std::string> seeds;

  for (auto& [ origin, targets ] : instructions) {
    if (origin.back() == 'A') {
      seeds.push_back(origin);
    }
  }

  auto compute_length = [&](const std::string& start) {
    std::size_t length = 0;
    std::string current = start;

    for (;;) {
      for (auto direction : lr) {
        if (direction == 'L') {
          current = instructions[current].first;
        } else {
          assert(direction == 'R');
          current = instructions[current].second;
        }

        ++length;

        if (current.back() == 'Z') {
          return length;
        }
      }
    }

    return length;
  };


  std::size_t result = 1;

  for (auto& current : seeds) {
    result = std::lcm(result, compute_length(current));
  }

  std::cout << result << '\n';
}
