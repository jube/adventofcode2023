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

  struct Vec3 {
    int x;
    int y;
    int z;
  };

  struct Brick {
    Vec3 min;
    Vec3 max;
    std::set<std::size_t> above;
    std::set<std::size_t> beyond;
  };

  struct Stack {
    int top = 0;
    std::size_t brick = 0;
  };

}

int main() {

  std::vector<Brick> bricks;

  Brick ground = {};
  ground.min.z = 0;
  ground.max.z = 0;

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.empty() || line == "\n") {
      continue;
    }

    Brick brick;

    auto parts = split_string(line, '~');
    assert(parts.size() == 2);

    auto min = split_string(parts[0], ',');
    assert(min.size() == 3);

    brick.min.x = std::stoi(std::string(min[0]));
    brick.min.y = std::stoi(std::string(min[1]));
    brick.min.z = std::stoi(std::string(min[2]));

    auto max = split_string(parts[1], ',');
    assert(max.size() == 3);

    brick.max.x = std::stoi(std::string(max[0]));
    brick.max.y = std::stoi(std::string(max[1]));
    brick.max.z = std::stoi(std::string(max[2]));

    bricks.push_back(brick);
  }

  for (auto& brick : bricks) {
    if (brick.min.x > brick.max.x) {
      std::swap(brick.min.x, brick.max.x);
    }

    ground.min.x = std::min(ground.min.x, brick.min.x);
    ground.max.x = std::max(ground.max.x, brick.max.x);

    if (brick.min.y > brick.max.y) {
      std::swap(brick.min.y, brick.max.y);
    }

    ground.min.y = std::min(ground.min.y, brick.min.y);
    ground.max.y = std::max(ground.max.y, brick.max.y);

    if (brick.min.z > brick.max.z) {
      std::swap(brick.min.z, brick.max.z);
    }
  }

  bricks.push_back(ground);

  std::stable_sort(bricks.begin(), bricks.end(), [](const Brick& lhs, const Brick& rhs) {
    return lhs.min.z < rhs.min.z;
  });

  // for (auto& brick : bricks) {
  //   std::cout << ' ' <<  brick.min.z;
  // }
  // std::cout << '\n';

  std::size_t dx = ground.max.x - ground.min.x + 1;
  std::size_t dy = ground.max.y - ground.min.y + 1;

  std::cout << "dx = " << dx << ", dy = " << dy << '\n';

  std::vector<std::vector<Stack>> stacks(dx, std::vector<Stack>(dy));

  for (std::size_t i = 1; i < bricks.size(); ++i) {
    auto& brick = bricks[i];

    int top = 0;

    for (int x = brick.min.x; x <= brick.max.x; ++x) {
      for (int y = brick.min.y; y <= brick.max.y; ++y) {
        top = std::max(stacks[x][y].top, top);
      }
    }

    for (int x = brick.min.x; x <= brick.max.x; ++x) {
      for (int y = brick.min.y; y <= brick.max.y; ++y) {
        assert(stacks[x][y].brick < i);

        if (top == stacks[x][y].top) {
          bricks[stacks[x][y].brick].above.insert(i);
          brick.beyond.insert(stacks[x][y].brick);
        }

        stacks[x][y].top = top + (brick.max.z - brick.min.z + 1);
        stacks[x][y].brick = i;
      }
    }

    // for (std::size_t yy = 0; yy < dy; ++yy) {
    //   for (std::size_t xx = 0; xx < dx; ++xx) {
    //     std::cout << stacks[xx][yy].brick;
    //   }
    //   std::cout << '\n';
    // }
    //
    // std::cout << '\n';

  }

  int count = 0;

  for (std::size_t j = 0; j < bricks.size(); ++j) {
    auto& brick = bricks[j];

    if (brick.above.empty()) {
      // std::cout << "Brick " << j << " can be disintegrated (none above)\n";
      ++count;
      continue;
    }

    if (std::all_of(brick.above.begin(), brick.above.end(), [&](std::size_t i) { return bricks[i].beyond.size() > 1; })) {
      // std::cout << "Brick " << j << " can be disintegrated (other support)\n";
      ++count;
    }
  }

  std::cout << "Count: " << count << '\n';
}
