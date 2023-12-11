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
  constexpr long ExpansionFactor = 1'000'000;

  std::vector<std::string> space;

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.empty() || line == "\n") {
      continue;
    }

    space.push_back(line);
  }

  std::size_t width = space.front().size();
  std::size_t height = space.size();

  std::vector<std::size_t> v_expansions;

  for (std::size_t i = 0; i < height; ++i) {
    if (space[i] == std::string(width, '.')) {
      v_expansions.push_back(i);
      std::cout << "Expansion at line: " << i << '\n';
    }
  }

  std::vector<std::size_t> h_expansions;

  for (std::size_t i = 0; i < width; ++i) {
    if (std::all_of(space.begin(), space.end(), [i](const std::string& line) { return line[i] == '.'; })) {
      h_expansions.push_back(i);
      std::cout << "Expansion at column: " << i << '\n';
    }
  }

  std::vector<std::pair<long, long>> locations;

  for (std::size_t j = 0; j < height; ++j) {
    for (std::size_t i = 0; i < width; ++i) {
      if (space[j][i] == '#') {
        locations.emplace_back(i, j);
      }
    }
  }

  auto distance = [&](std::size_t i, std::size_t j) {
    long length = std::abs(locations[i].first - locations[j].first) + std::abs(locations[i].second - locations[j].second);

    auto [ ymin, ymax ] = std::minmax(locations[i].second, locations[j].second);
    auto v_first = std::lower_bound(v_expansions.begin(), v_expansions.end(), ymin);
    auto v_last = std::upper_bound(v_expansions.begin(), v_expansions.end(), ymax);
    auto v_length = (v_last - v_first) * (ExpansionFactor - 1);

    auto [ xmin, xmax ] = std::minmax(locations[i].first, locations[j].first);
    auto h_first = std::lower_bound(h_expansions.begin(), h_expansions.end(), xmin);
    auto h_last = std::upper_bound(h_expansions.begin(), h_expansions.end(), xmax);
    auto h_length = (h_last - h_first) * (ExpansionFactor - 1);

    std::cout << "Length: " << length << ", v_length: " << v_length << ", h_length: " << h_length << '\n';

    return length + v_length + h_length;
  };

  std::size_t count = locations.size();
  long sum = 0;

  for (std::size_t i = 0; i < count; ++i) {
    for (std::size_t j = i + 1; j < count; ++j) {
      int length = distance(i, j);
      std::cout << "Length between " << i + 1 << " and " << j + 1 << ": " << length << '\n';
      sum += length;
    }
  }

  std::cout << "Sum: " << sum << '\n';
}
