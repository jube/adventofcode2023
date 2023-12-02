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
  int sum = 0;

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.empty() || line == "\n") {
      continue;
    }

    auto parts = split_string(line, ":");
    assert(parts.size() == 2);

    auto sets = split_string(parts[1], ";");

    int max_red = 0;
    int max_green = 0;
    int max_blue = 0;

    for (auto& set : sets) {
      auto colored = split_string(set, ",");

      int red = 0;
      int green = 0;
      int blue = 0;

      for (auto& cube : colored) {
        std::string value_string(cube);
        int value = std::stoi(value_string);

        std::cout << cube << ':' << value << '\n';

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

      max_red = std::max(max_red, red);
      max_green = std::max(max_green, green);
      max_blue = std::max(max_blue, blue);
    }

    int power = max_red * max_green * max_blue;
    sum += power;
  }

  std::cout << sum << '\n';
}
