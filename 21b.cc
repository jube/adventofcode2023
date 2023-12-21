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

  constexpr int Steps = 26501365;
  // constexpr int Steps = 5000;

  std::vector<std::string> ground;

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.empty() || line == "\n") {
      continue;
    }

    ground.push_back(line);
  }

  const std::size_t size = ground.size();

  for (std::size_t y = 0; y < size; ++y) {
    for (std::size_t x = 0; x < size; ++x) {
      if (ground[y][x] == 'S') {
        ground[y][x] = '.';
        break;
      }
    }
  }

  std::vector<std::string> maxi;

  constexpr std::size_t Factor = 9;

  for (std::size_t i = 0; i < size; ++i) {
    maxi.push_back("");

    for (std::size_t k = 0; k < Factor; ++k) {
      maxi[i] += ground[i];
    }
  }

  for (std::size_t k = 1; k < Factor; ++k) {
    for (std::size_t i = 0; i < size; ++i) {
      std::string line = maxi[i];
      maxi.push_back(line);
    }
  }

  const std::size_t maxisize = Factor * size;
  assert(maxi.size() == maxisize);

  std::size_t startx = (maxisize - 1) / 2;
  std::size_t starty = (maxisize - 1) / 2;

  std::cout << "start: " << startx << '\n';

  assert(2 * startx + 1 == maxisize);
  assert(2 * starty + 1 == maxisize);

  auto count_plots = [&] (std::size_t steps) {
    auto map = maxi;
    map[starty][startx] = 'O';

    for (std::size_t i = 0; i < steps; ++i) {
      auto next = maxi;

      for (std::size_t y = 0; y < maxisize; ++y) {
        for (std::size_t x = 0; x < maxisize; ++x) {

          char c = map[y][x];

          if (c == '#') {
            continue;
          }

          if (c == 'O') {
            if (x > 0 && map[y][x - 1] != '#') {
              next[y][x - 1] = 'O';
            }

            if (x < maxisize - 1 && map[y][x + 1] != '#') {
              next[y][x + 1] = 'O';
            }

            if (y > 0 && map[y - 1][x] != '#') {
              next[y - 1][x] = 'O';
            }

            if (y < maxisize - 1 && map[y + 1][x] != '#') {
              next[y + 1][x] = 'O';
            }
          }

        }
      }

      map = next;
    }

    long count = 0;

    for (auto& line : map) {
      for (auto& c : line) {
        if (c == 'O') {
          ++count;
        }
      }
    }

    return count;
  };

  auto test = [&](std::size_t steps) {
    auto count = count_plots(steps);
    std::cout << "Test with " << steps << ": " << count << '\n';
    return count;
  };

  std::size_t steps = Steps % size;

  std::vector<long> measures;
  measures.push_back(test(steps));

  for (std::size_t i = 0; i < Factor / 2; ++i) {
    steps += size;
    measures.push_back(test(steps));
  }

  std::vector<long> speed;
  std::adjacent_difference(measures.begin(), measures.end(), std::back_inserter(speed));

  std::cout << "Speeds:";
  for (auto& s : speed) {
    std::cout << ' ' << s;
  }
  std::cout << '\n';

  std::vector<long> acceleration;
  std::adjacent_difference(speed.begin(), speed.end(), std::back_inserter(acceleration));

  std::cout << "Accelerations:";
  for (auto& a : acceleration) {
    std::cout << ' ' << a;
  }
  std::cout << '\n';

  std::size_t giant_steps_left = (Steps - (Steps % size) - (measures.size() - 1) * size);
  assert(giant_steps_left % size == 0);
  giant_steps_left /= size;

  std::cout << "Giant Steps Left: " << giant_steps_left << '\n';

  long s = speed.back();
  long total = measures.back();

  for (std::size_t i = 0; i < giant_steps_left; ++i) {
    s += acceleration.back();
    total += s;
  }

  std::cout << "Total: " << total << '\n';

}
