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

  using Automaton = std::map<int, std::map<char, std::vector<int>>>;

  Automaton build_automaton(const std::vector<std::size_t>& pattern) {
    Automaton automaton;
    int current_state = 0;

    for (std::size_t j = 0; j < pattern.size(); ++j) {
      auto count = pattern[j];

      automaton[current_state]['.'].emplace_back(current_state);
      automaton[current_state]['?'].emplace_back(current_state);

      for (std::size_t i = 0; i < count; ++i) {
        automaton[current_state]['#'].emplace_back(current_state + 1);
        automaton[current_state]['?'].emplace_back(current_state + 1);
        ++current_state;
      }

      if (j < pattern.size() - 1) {
        automaton[current_state]['.'].emplace_back(current_state + 1);
        automaton[current_state]['?'].emplace_back(current_state + 1);
        ++current_state;
      }
    }

    automaton[current_state]['.'].emplace_back(current_state);
    automaton[current_state]['?'].emplace_back(current_state);

    return automaton;
  }

  using Memory = std::map<std::tuple<int, std::size_t, char>, long>;

  long compute_count(Automaton& automaton, Memory& memory, const std::string& word, std::size_t i, int state) {
    if (i == word.size()) {
      return (static_cast<std::size_t>(state) == automaton.size() - 1) ? 1 : 0;
    }

    char c = word[i];

    if (auto iterator = memory.find(std::make_tuple(state, i, c)); iterator != memory.end()) {
      // std::cout << "Found (" << state << ',' << i << ',' << c << ") = " << iterator->second << '\n';
      return iterator->second;
    }

    long sum = 0;

    for (auto target : automaton[state][c]) {
      sum += compute_count(automaton, memory, word, i + 1, target);
    }

    // std::cout << "Insert (" << state << ',' << i << ',' << c << ") = " << sum << '\n';
    memory.emplace(std::make_tuple(state, i, c), sum);
    return sum;
  }

  long compute(const SpringRow& row) {
    auto automaton = build_automaton(row.pattern);
    Memory memory;
    return compute_count(automaton, memory, row.springs, 0, 0);
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

    for (int i = 0; i < 5; ++i) {
      row.springs += parts[0];
      row.springs += '?';
    }

    row.springs.pop_back();
    row.springs.push_back('.');

    auto ints = split_string(parts[1], ',');

    for (int i = 0; i < 5; ++i) {
      std::transform(ints.begin(), ints.end(), std::back_inserter(row.pattern), [](const std::string_view& str) { return std::stoul(std::string(str)); });
    }

    rows.push_back(std::move(row));
  }

  long sum = 0;

  for (auto& row : rows) {
    long total = compute(row);
    std::cout << "Total: " << total << '\n';
    sum += total;
  }

  std::cout << "Sum: " << sum << '\n';
}
