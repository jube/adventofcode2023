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

  std::vector<std::string_view> split_string(std::string_view string, char delimiter)
  {
    const std::size_t size = string.size();
    std::size_t i = 0;
    std::vector<std::string_view> result;

    while (i < size) {
      while (i < size && string[i] == delimiter) {
        ++i;
      }

      if (i == size) {
        break;
      }

      const char* start = string.data() + i;
      size_t length = 0;

      while (i < size && string[i] != delimiter) {
        ++i;
        ++length;
      }

      result.emplace_back(start, length);
    }

    return result;
  }

  struct SpringRow {
    std::string springs;
    std::vector<std::size_t> pattern;
  };


  long explore(const SpringRow& row, std::size_t i, std::size_t count, std::size_t j) {
    if (i == row.springs.size()) {
      if (j == row.pattern.size()) {
        return 1;
      }

      return 0;
    }

    assert(i < row.springs.size());

    if (row.springs[i] == '.') {
      if (count > 0) {
        if (j < row.pattern.size()) {
          if (row.pattern[j] == count) {
            return explore(row, i + 1, 0, j + 1);
          }
        }

        // bad
        return 0;
      }

      return explore(row, i + 1, count, j);
    }

    if (row.springs[i] == '#') {
      return explore(row, i + 1, count + 1, j);
    }

    assert(row.springs[i] == '?');
    SpringRow copy = row;

    long sum = 0;

    for (char c : { '.', '#' }) {
      copy.springs[i] = c;
      sum += explore(copy, i, count, j);
    }

    return sum;
  }


}

int main() {
  std::vector<SpringRow> rows;

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.empty() || line == "\n") {
      continue;
    }

    auto parts = split_string(line, ' ');
    assert(parts.size() == 2);

    SpringRow row;
    row.springs = parts[0];
    row.springs.push_back('.');

    auto ints = split_string(parts[1], ',');
    std::transform(ints.begin(), ints.end(), std::back_inserter(row.pattern), [](const std::string_view& str) { return std::stoul(std::string(str)); });

    rows.push_back(std::move(row));
  }

  long sum = 0;

  for (auto& row : rows) {
    long total = explore(row, 0, 0, 0);
    std::cout << "Total: " << total << '\n';
    sum += total;
  }

  std::cout << "Sum: " << sum << '\n';
}
