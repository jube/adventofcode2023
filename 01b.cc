#include <cassert>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>

constexpr std::tuple<std::string_view, int> numbers[] = {
  { "one", 1 },
  { "two", 2 },
  { "three", 3 },
  { "four", 4 },
  { "five", 5 },
  { "six", 6 },
  { "seven", 7 },
  { "eight", 8 },
  { "nine", 9 },
};

int main() {
  int sum = 0;

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.empty() || line == "\n") {
      continue;
    }

    bool first = true;
    int value0 = 0;
    int value1 = 0;
    std::size_t ith = 0;

    for (char c : line) {
      if ('0' <= c && c <= '9') {
        if (first) {
          value0 = (c - '0');
          first = false;
        }

        value1 = (c - '0');
      } else {
        assert('a' <= c && c <= 'z');

        for (auto [ rep, number ] : numbers) {
          if (line.substr(ith, rep.size()) == rep) {
            if (first) {
              value0 = number;
              first = false;
            }

            value1 = number;
          }
        }
      }

      ++ith;
    }

    int value = value0 * 10 + value1;
    std::cout << value0 << ' ' << value1 << ' ' << value << '\n';
    sum += value;
  }

  std::cout << sum << '\n';
}
