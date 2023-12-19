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

  struct Input {
    char dir;
    int length;
    std::string color;
  };

  void fill(std::vector<char>& ground, int width, int height, int x, int y) {
    auto g = [&ground, width, height](int x, int y) -> char& {
      assert(0 <= x && x < width);
      assert(0 <= y && y < height);
      return ground[y * width + x];
    };

    if (x > 0 && g(x - 1, y) == '.') {
      g(x - 1, y) = '#';
      fill(ground, width, height, x - 1, y);
    }

    if (x < width - 1 && g(x + 1, y) == '.') {
      g(x + 1, y) = '#';
      fill(ground, width, height, x + 1, y);
    }

    if (y > 0 && g(x, y - 1) == '.') {
      g(x, y - 1) = '#';
      fill(ground, width, height, x, y - 1);
    }

    if (y < height - 1 && g(x, y + 1) == '.') {
      g(x, y + 1) = '#';
      fill(ground, width, height, x, y + 1);
    }
  }

}

int main() {
  std::vector<Input> inputs;

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.empty() || line == "\n") {
      continue;
    }

    auto parts = split_string(line, ' ');
    assert(parts.size() == 3);

    Input input;
    input.dir = parts[0][0];
    input.length = std::stoi(std::string(parts[1]));
    input.color = parts[2].substr(2, 6);

    inputs.push_back(std::move(input));
  }

  int xmin = 0;
  int xmax = 0;
  int ymin = 0;
  int ymax = 0;

  int x = 0;
  int y = 0;

  for (auto& input : inputs) {
    switch (input.dir) {
      case 'U':
        y -= input.length;
        if (y < ymin) { ymin = y; }
        break;
      case 'D':
        y += input.length;
        if (y > ymax) { ymax = y; }
        break;
      case 'L':
        x -= input.length;
        if (x < xmin) { xmin = x; }
        break;
      case 'R':
        x += input.length;
        if (x > xmax) { xmax = x; }
        break;
      default:
        assert(false);
        break;
    }
  }

  std::cout << "x in [" << xmin << ',' << xmax << "], y in [" << ymin << ',' << ymax << "]\n";

  int width = xmax - xmin + 1;
  int height = ymax - ymin + 1;

  std::vector<char> ground(width * height, '.');

  auto g = [&ground, width, height](int x, int y) -> char& {
    assert(0 <= x && x < width);
    assert(0 <= y && y < height);
    return ground[y * width + x];
  };

  auto print = [&]() {
    for (int y = 0; y < height; ++y) {
      for (int x = 0; x < width; ++x) {
        std::cout << g(x, y);
      }

      std::cout << '\n';
    }
  };

  x = 0-xmin;
  y = 0-ymin;

  std::cout << "start: " << x << ',' << y << '\n';

  g(x, y) = '#';

  for (auto& input : inputs) {
    int dx = 0;
    int dy = 0;

    std::cout << "pos: " << x << ',' << y << '\n';

    switch (input.dir) {
      case 'U':
        dy = -1;
        break;
      case 'D':
        dy = 1;
        break;
      case 'L':
        dx = -1;
        break;
      case 'R':
        dx = 1;
        break;
      default:
        assert(false);
        break;
    }

    for (int i = 0; i < input.length; ++i) {
      x += dx;
      y += dy;
      g(x, y) = '#';
    }
  }

  fill(ground, width, height, width / 2, height / 2);

  print();

  std::cout << "Lava: " << std::count(ground.begin(), ground.end(), '#') << '\n';

}
