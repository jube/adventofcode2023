#include <cassert>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>

int main() {
  int sum = 0;

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.empty() || line == "\n") {
      continue;
    }

    bool first = true;
    int value0 = 0;
    int value1 = 0;

    for (char c : line) {
      if ('0' <= c && c <= '9') {
        if (first) {
          value0 = (c - '0');
          first = false;
        }

        value1 = (c - '0');
      }
    }

    int value = value0 * 10 + value1;
    // std::cout << value0 << ' ' << value1 << ' ' << value << '\n';
    sum += value;
  }

  std::cout << sum << '\n';
}
