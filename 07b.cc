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

  struct Hand {
    std::string hand;
    std::string true_hand;
    int bid;
  };

  enum class Rank {
    High,
    OnePair,
    TwoPairs,
    Three,
    Full,
    Four,
    Five,
  };

  Rank compute_rank(std::string hand);

  void true_hand_rec(const std::string& original, std::string& current, std::size_t i, std::vector<std::string>& hands) {
    if (i == 5) {
      hands.push_back(current);
      return;
    }

    if (original[i] != 'J') {
      current[i] = original[i];
      true_hand_rec(original, current, i + 1, hands);
      return;
    }

    for (char c : "AKQT98765432") {
      current[i] = c;
      true_hand_rec(original, current, i + 1, hands);
    }
  }


  std::string true_hand(const std::string& original)
  {
    std::vector<std::string> hands;
    std::string current = original;
    true_hand_rec(original, current, 0, hands);

    return *std::max_element(hands.begin(), hands.end(), [](const std::string& lhs, const std::string& rhs) {
      return compute_rank(lhs) < compute_rank(rhs);
    });
  }


  Rank compute_rank(std::string hand) {
    assert(hand.size() == 5);

    std::map<char, int> cards;

    for (auto c : hand) {
      cards[c]++;
    }

    std::vector<int> count;

    for (auto [ c, x ] : cards) {
      count.push_back(x);
    }

    std::sort(count.begin(), count.end());

    if (count.size() == 1) {
      assert(count[0] == 5);
      return Rank::Five;
    }

    if (count.size() == 2) {
      if (count[0] == 1 && count[1] == 4) {
        return Rank::Four;
      }

      if (count[0] == 2 && count[1] == 3) {
        return Rank::Full;
      }

      assert(false);
    }

    if (count.size() == 3) {
      if (count[0] == 1 && count[1] == 1 && count[2] == 3) {
        return Rank::Three;
      }

      if (count[0] == 1 && count[1] == 2 && count[2] == 2) {
        return Rank::TwoPairs;
      }

      std::cout << count[0] << ' ' << count[1] << ' ' << count[2] << '\n';
      assert(false);
    }

    if (count.size() == 4) {
      if (count[0] == 1 && count[1] == 1 && count[2] == 1 && count[3] == 2) {
        return Rank::OnePair;
      }

      assert(false);
    }

    if (count.size() == 5) {
      if (count[0] == 1 && count[1] == 1 && count[2] == 1 && count[3] == 1 && count[4] == 1) {
        return Rank::High;
      }

      assert(false);
    }

    assert(false);
    return Rank::High;
  }

  bool compute_order(char lhs, char rhs)
  {
    const std::map<char, int> order = {
      { 'A', 1 },
      { 'K', 2 },
      { 'Q', 3 },
      { 'T', 5 },
      { '9', 6 },
      { '8', 7 },
      { '7', 8 },
      { '6', 9 },
      { '5', 10 },
      { '4', 11 },
      { '3', 12 },
      { '2', 13 },
      { 'J', 14 },
    };

    auto it_lhs = order.find(lhs);
    assert(it_lhs != order.end());

    auto it_rhs = order.find(rhs);
    assert(it_rhs != order.end());

    return it_lhs->second > it_rhs->second;
  }

  bool comparator(const Hand& lhs, const Hand& rhs)
  {
    assert(lhs.hand.size() == 5);
    Rank lhs_rank = compute_rank(lhs.true_hand);

    assert(rhs.hand.size() == 5);
    Rank rhs_rank = compute_rank(rhs.true_hand);

    if (lhs_rank != rhs_rank) {
      return lhs_rank < rhs_rank;
    }

    for (std::size_t i = 0; i < 5; ++i) {
      if (lhs.hand[i] != rhs.hand[i]) {
        return compute_order(lhs.hand[i], rhs.hand[i]);
      }
    }

    assert(false);
    return true;
  }

}



int main() {

  std::vector<Hand> hands;

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.empty() || line == "\n") {
      continue;
    }

    auto parts = split_string(line, " ");
    assert(parts.size() == 2);

    Hand hand;
    hand.hand = std::string(parts[0]);
    assert(hand.hand.size() == 5);
    hand.true_hand = true_hand(hand.hand);
    hand.bid = std::stoi(std::string(parts[1]));
    hands.push_back(std::move(hand));
    assert(hands.back().hand.size() == 5);
  }

  std::cout << hands.size() << '\n';

  std::sort(hands.begin(), hands.end(), comparator);

  int i = 1;
  int sum = 0;

  for (auto& hand : hands) {
    std::cout << hand.hand << ' ' << hand.bid << '\n';
    sum += i * hand.bid;
    ++i;
  }

  std::cout << sum << '\n';
}
