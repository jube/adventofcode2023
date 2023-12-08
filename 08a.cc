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

  int steps = 0;
  std::string origin = "AAA";
  std::string current = origin;

  for (;;) {

    for (auto direction : lr) {
      if (direction == 'L') {
        current = instructions[current].first;
      } else {
        assert(direction == 'R');
        current = instructions[current].second;
      }

      ++steps;

      if (current == "ZZZ") {
        std::cout << steps << '\n';
        return 0;
      }
    }

  }

}
