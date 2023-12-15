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

  struct Lens {
    std::string_view label;
    unsigned long focal_length = 0;
  };

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

  std::vector<std::vector<Lens>> boxes(256);

  [[maybe_unused]] auto display = [&boxes]() {
    for (std::size_t i = 0; i < boxes.size(); ++i) {
      auto& box = boxes[i];

      if (box.empty()) {
        continue;
      }

      std::cout << "Box " << i << ":";

      for (auto& lens : box) {
        std::cout << " [" << lens.label << ' ' << lens.focal_length << ']';
      }

      std::cout << '\n';
    }

  };

  for (auto part : parts) {
    if (part.back() == '-') {
      part.remove_suffix(1);
      uint8_t h = compute_hash(part);
      auto& box = boxes[h];

      box.erase(std::remove_if(box.begin(), box.end(), [&](auto& lens) { return lens.label == part; }), box.end());

    } else {
      auto subparts = split_string(part, '=');
      assert(subparts.size() == 2);

      uint8_t h = compute_hash(subparts[0]);
      auto& box = boxes[h];

      if (auto iterator = std::find_if(box.begin(), box.end(), [&](const Lens& lens) { return lens.label == subparts[0]; }); iterator != box.end()) {
        iterator->focal_length = std::stoul(std::string(subparts[1]));
      } else {
        box.push_back({ subparts[0], std::stoul(std::string(subparts[1])) });
      }
    }

    // display();
    // std::cout << '\n';
  }

  unsigned long power = 0;

  for (std::size_t i = 0; i < boxes.size(); ++i) {
    auto& box = boxes[i];

    for (std::size_t j = 0; j < box.size(); ++j) {
      auto focusing_power = (i + 1) * (j + 1) * box[j].focal_length;

      // std::cout << "- " << focusing_power << '\n';

      power += focusing_power;
    }
  }

  std::cout << "Power: " << power << '\n';
}
