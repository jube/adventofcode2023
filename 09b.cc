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

  long compute_differences(const std::vector<long>& values) {
    for (auto value : values) {
      std::cout << '\t' << value;
    }
    std::cout << '\n';

    std::vector<long> differences;
    std::adjacent_difference(values.begin(), values.end(), std::back_inserter(differences));
    differences.erase(differences.begin());

    if (std::all_of(differences.begin(), differences.end(), [](long value) { return value == 0; })) {
      return 0;
    }

    long first = compute_differences(differences);
    std::cout << "First: " << first << '\n';
    return differences.front() - first;
  }

}

int main() {
  long sum = 0;

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.empty() || line == "\n") {
      continue;
    }

    auto parts = split_string(line, " ");
    std::vector<long> initial_values;

    for (auto part : parts) {
      initial_values.push_back(std::stol(std::string(part)));
    }

    long first = compute_differences(initial_values);
    std::cout << "First: " << first << '\n';
    long prediction = initial_values.front() - first;
    std::cout << "Prediction: " << prediction << '\n';
    sum += prediction;
  }

  std::cout << sum << '\n';
}
