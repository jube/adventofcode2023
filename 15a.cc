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

  uint8_t compute_hash(std::string_view str) {
    uint8_t h = 0;

    for (auto c : str) {
      h += static_cast<uint8_t>(c);
      h *= 17;
    }

    return h;
  }

}

int main() {
  std::string input;

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.empty() || line == "\n") {
      continue;
    }

    input = line;
  }

  auto parts = split_string(input, ',');

  unsigned long hash = 0;

  for (auto part : parts) {
    hash += compute_hash(part);
  }


  std::cout << "Hash: " << hash << '\n';
}
