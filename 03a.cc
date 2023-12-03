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

  auto check = [&](std::size_t j, std::size_t start, std::size_t end) {
    if (start > 0) {
      --start;
    }

    if (end < width) {
      ++end;
    }

    for (std::size_t i = start; i < end; ++i) {
      char c = lines[j][i];

      if (c == '.' || is_digit(c)) {
        continue;
      }

      return true;
    }

    return false;
  };

  int sum = 0;

  for (std::size_t j = 0; j < height; ++j) {
    assert(lines[j].size() == width);

    std::cout << "==== line " << j << '\n';

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

            bool has_adjacent_symbol = false;

            if (j > 0) {
              has_adjacent_symbol = has_adjacent_symbol || check(j - 1, start, end);
            }

            has_adjacent_symbol = has_adjacent_symbol || check(j, start, end);

            if (j < height - 1) {
              has_adjacent_symbol = has_adjacent_symbol || check(j + 1, start, end);
            }

            if (has_adjacent_symbol) {
              std::cout << number << " has adjacent symbol\n";
              sum += number;
            } else {
              std::cout << number << " has *not* adjacent symbol\n";
            }

            number = 0;
          }
          break;
      }
    }

    if (state == State::Number) {
      end = width;

      bool has_adjacent_symbol = false;

      if (j > 0) {
        has_adjacent_symbol = has_adjacent_symbol || check(j - 1, start, end);
      }

      has_adjacent_symbol = has_adjacent_symbol || check(j, start, end);

      if (j < height - 1) {
        has_adjacent_symbol = has_adjacent_symbol || check(j + 1, start, end);
      }

      if (has_adjacent_symbol) {
        std::cout << number << " has adjacent symbol\n";
        sum += number;
      } else {
        std::cout << number << " has *not* adjacent symbol\n";
      }
    }
  }

  std::cout << sum << '\n';
}
