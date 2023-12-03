#include <cassert>
#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>

namespace {

  bool is_digit(char c)
  {
    return '0' <= c && c <= '9';
  }

}

enum class State {
  Void,
  Number,
};

int main() {
  std::vector<std::string> lines;

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.empty() || line == "\n") {
      continue;
    }

    lines.push_back(line);
  }

  const std::size_t height = lines.size();
  const std::size_t width = lines.front().size();

  std::map<std::tuple<int, int>, std::vector<int>> gears;

  auto check = [&](std::size_t j, std::size_t start, std::size_t end, int number) {
    if (start > 0) {
      --start;
    }

    if (end < width) {
      ++end;
    }

    for (std::size_t i = start; i < end; ++i) {
      char c = lines[j][i];

      if (c == '*') {
        gears[std::make_tuple(i, j)].push_back(number);
        std::cout << "gears at " << i << 'x' << j << ": " << number << '\n';
      }
    }
  };

  for (std::size_t j = 0; j < height; ++j) {
    assert(lines[j].size() == width);

    int number = 0;
    std::size_t start = 0;
    std::size_t end = 0;
    State state = is_digit(lines.front().front()) ? State::Number : State::Void;

    for (std::size_t i = 0; i < width; ++i) {
      const char c = lines[j][i];

      switch (state) {
        case State::Void:
          if (is_digit(c)) {
            // start of a number
            state = State::Number;
            start = i;
            number = c - '0';
          }
          break;
        case State::Number:
          if (is_digit(c)) {
            number = 10 * number + (c - '0');
          } else {
            // end of the number
            state = State::Void;
            end = i;

            if (j > 0) {
              check(j - 1, start, end, number);
            }

            check(j, start, end, number);

            if (j < height - 1) {
              check(j + 1, start, end, number);
            }

            number = 0;
          }
          break;
      }
    }

    if (state == State::Number) {
      end = width;

      if (j > 0) {
        check(j - 1, start, end, number);
      }

      check(j, start, end, number);

      if (j < height - 1) {
        check(j + 1, start, end, number);
      }
    }
  }

  int sum = 0;

  for (auto& [k, v] : gears) {
    if (v.size() == 2) {
      sum += v[0] * v[1];
    }
  }

  std::cout << sum << '\n';
}
