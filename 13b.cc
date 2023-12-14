#include <cassert>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <numeric>
#include <optional>
#include <set>
#include <string>
#include <tuple>
#include <vector>

int smudge(const std::string& lhs, const std::string& rhs) {
  assert(lhs.size() == rhs.size());

  int res = 0;

  for (std::size_t i = 0; i < lhs.size(); ++i) {
    if (lhs[i] != rhs[i]) {
      res++;
    }
  }

  return res;
}


void compute_h_reflection(const std::vector<std::string>& pattern, std::size_t& sum) {
  std::size_t height = pattern.size();

  for (std::size_t x = 1; x < height; ++x) {
    std::size_t min = std::min(x, height - x);
    int count = 0;

    for (std::size_t y = 0; y < min; ++y) {
      count += smudge(pattern[x - y - 1], pattern[x + y]);

      if (count > 1) {
        break;
      }
    }

    if (count == 1) {
      std::cout << "H: " << x << '\n';
      sum += 100 * x;
    }

  }
}

void compute_v_reflection(const std::vector<std::string>& pattern, std::size_t& sum) {
  std::size_t width = pattern[0].size();

  for (std::size_t x = 1; x < width; ++x) {
    int count = 0;

    for (auto& line : pattern) {
      std::string left = line.substr(0, x);
      std::string right = line.substr(x);
      std::reverse(left.begin(), left.end());

      std::size_t min = std::min(left.size(), right.size());

      // std::cout << "x: " << x << ", min: " << min << '\n';

      left.resize(min);
      right.resize(min);

      count += smudge(left, right);

      if (count > 1) {
        break;
      }
    }

    if (count == 1) {
      std::cout << "V: " << x << '\n';
      sum += x;
    }
  }
}

int main() {
  std::vector<std::vector<std::string>> patterns;
  std::vector<std::string> pattern;

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.empty() || line == "\n") {
      patterns.push_back(std::move(pattern));
      pattern.clear();
      continue;
    }

    pattern.push_back(line);
  }

  assert(!pattern.empty());
  patterns.push_back(pattern);

  std::cout << patterns.size() << '\n';

  std::size_t sum = 0;

  for (auto& p : patterns) {
    std::cout << "----\n";
    compute_v_reflection(p, sum);
    compute_h_reflection(p, sum);
  }

  std::cout << "Sum: " << sum << '\n';
}
