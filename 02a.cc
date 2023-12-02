#include <cassert>
#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>

namespace {
  bool is_delimiter(char c, std::string_view delimiters)
  {
    return std::any_of(delimiters.begin(), delimiters.end(), [c](char d) { return c == d; });
  }

  std::vector<std::string_view> split_string(std::string_view string, std::string_view delimiters)
  {
    const std::size_t size = string.size();
    std::size_t i = 0;
    std::vector<std::string_view> result;

    while (i < size) {
      while (i < size && is_delimiter(string[i], delimiters)) {
        ++i;
      }

      if (i == size) {
        break;
      }

      const char* start = string.data() + i;
      size_t length = 0;

      while (i < size && !is_delimiter(string[i], delimiters)) {
        ++i;
        ++length;
      }

      result.emplace_back(start, length);
    }

    return result;
  }

}

int main() {
  static constexpr int MaxRed = 12;
  static constexpr int MaxGreen = 13;
  static constexpr int MaxBlue = 14;

  int id = 1;
  int sum_id = 0;

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.empty() || line == "\n") {
      continue;
    }

    auto parts = split_string(line, ":");
    assert(parts.size() == 2);

    auto sets = split_string(parts[1], ";");
    bool possible = true;

    for (auto& set : sets) {
      auto colored = split_string(set, ",");

      int red = 0;
      int green = 0;
      int blue = 0;

      for (auto& cube : colored) {
        std::string value_string(cube);
        int value = std::stoi(value_string);

        // std::cout << cube << ':' << value << '\n';

        if (cube.find("red") != std::string_view::npos) {
          red += value;
        } else if (cube.find("green") != std::string_view::npos) {
          green += value;
        } else if (cube.find("blue") != std::string_view::npos) {
          blue += value;
        } else {
          assert(false);
        }
      }

      if (red > MaxRed || green > MaxGreen || blue > MaxBlue) {
        possible = false;
      }
    }

    if (possible) {
      sum_id += id;
    }

    ++id;
  }

  std::cout << sum_id << '\n';
}
