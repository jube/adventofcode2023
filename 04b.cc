#include <cassert>
#include <algorithm>
#include <iostream>
#include <iterator>
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

}

int main() {
  std::size_t card = 0;
  std::vector<std::size_t> cards_count(1000, 1);

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.empty() || line == "\n") {
      continue;
    }

    auto parts = split_string(line, ":");
    assert(parts.size() == 2);
    auto sets = split_string(parts[1], "|");
    assert(sets.size() == 2);

    auto winning_strings = split_string(sets[0], " ");
    std::set<int> winning;

    for (auto str : winning_strings) {
      winning.insert(std::stoi(std::string(str)));
    }

    auto number_strings = split_string(sets[1], " ");
    std::set<int> numbers;

    for (auto str : number_strings) {
      numbers.insert(std::stoi(std::string(str)));
    }

    std::vector<int> result;
    std::set_intersection(winning.begin(), winning.end(), numbers.begin(), numbers.end(), std::back_inserter(result));

    std::size_t matching = result.size();

    for (std::size_t i = 0; i < matching; ++i) {
      cards_count[card + i + 1] += cards_count[card];
    }

    ++card;

    std::cout << "Card " << card << ": " << cards_count[card - 1] << '\n';
  }

  cards_count.resize(card);

  std::cout << std::accumulate(cards_count.begin(), cards_count.end(), 0) << '\n';

}
