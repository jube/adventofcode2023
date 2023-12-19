#include <cassert>
#include <cstdint>
#include <algorithm>
#include <charconv>
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
    long length;
    std::string color;
  };

  long cross(long x0, long y0, long x1, long y1)
  {
    return x0 * y1 - y0 * x1;
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
    input.dir = "RDLU"[parts[2][7] - '0'];
    std::from_chars(parts[2].data() + 2, parts[2].data() + 7, input.length, 16);
    std::cout << "Dir: " << input.dir << ", Length: " << input.length << '\n';

    inputs.push_back(std::move(input));
  }

  long xmin = 0;
  long xmax = 0;
  long ymin = 0;
  long ymax = 0;

  long x = 0;
  long y = 0;

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

  x = 0;
  y = 0;

  std::cout << "start: " << x << ',' << y << '\n';

  long area = 0;
  long perimeter = 0;

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

    long xe = x + dx * input.length;
    long ye = y + dy * input.length;

    area += cross(x, y, xe, ye);
    perimeter +=  + input.length;

    x = xe;
    y = ye;
  }

  std::cout << "Area: " << 1 + perimeter / 2 + std::abs(area) / 2 << '\n';
}
