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

  std::vector<std::string> space;

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.empty() || line == "\n") {
      continue;
    }

    space.push_back(line);
  }

  std::size_t width = space.front().size();
  std::size_t height = space.size();

  std::vector<std::size_t> expansions;

  for (std::size_t i = 0; i < height; ++i) {
    if (space[i] == std::string(width, '.')) {
      std::size_t line = i + expansions.size();
      expansions.push_back(line);
      std::cout << "Expansion at line: " << i << '\n';
    }
  }

  for (auto i : expansions) {
    space.insert(space.begin() + i, std::string(width, '.'));
  }

  height += expansions.size();
  expansions.clear();

  for (std::size_t i = 0; i < width; ++i) {
    if (std::all_of(space.begin(), space.end(), [i](const std::string& line) { return line[i] == '.'; })) {
      std::size_t column = i + expansions.size();
      expansions.push_back(column);
      std::cout << "Expansion at column: " << i << '\n';
    }
  }

  for (auto& line : space) {
    for (auto i : expansions) {
      line.insert(line.begin() + i, '.');
    }
  }

  width += expansions.size();
  expansions.clear();

  for (auto& line : space) {
    std::cout << line << '\n';
  }

  std::vector<std::pair<int, int>> locations;

  for (std::size_t j = 0; j < height; ++j) {
    for (std::size_t i = 0; i < width; ++i) {
      if (space[j][i] == '#') {
        locations.emplace_back(i, j);
      }
    }
  }

  std::size_t count = locations.size();
  int sum = 0;

  for (std::size_t i = 0; i < count; ++i) {
    for (std::size_t j = i + 1; j < count; ++j) {
      int length = std::abs(locations[i].first - locations[j].first) + std::abs(locations[i].second - locations[j].second);
      std::cout << "Length between " << i + 1 << " and " << j + 1 << ": " << length << '\n';
      sum += length;
    }
  }

  std::cout << "Sum: " << sum << '\n';
}
