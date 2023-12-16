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

  struct Tile {
    char c = '.';
    uint8_t energized;
  };

  struct Vec {
    int x;
    int y;
  };

  struct Beam {
    Vec pos;
    Vec dir;
  };

  uint8_t bit(Vec v) {
    assert(v.x == 0 || v.y == 0);
    if (v.x == -1) {
      return 0x01;
    } else if (v.x == 1) {
      return 0x02;
    } else if (v.y == -1) {
      return 0x04;
    } else if (v.y == 1) {
      return 0x08;
    } else {
      assert(false);
    }

    return 0x00;
  }

}

int main() {

  std::vector<std::vector<Tile>> grid;

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.empty() || line == "\n") {
      continue;
    }

    std::vector<Tile> tile_line;

    for (auto c : line) {
      tile_line.push_back({ c, false });
    }

    grid.emplace_back(std::move(tile_line));
  }

  const int width = grid.front().size();
  const int height = grid.size();

  auto valid = [&](Beam beam) {
    if (0 <= beam.pos.x && beam.pos.x < width && 0 <= beam.pos.y && beam.pos.y < height) {
      return (grid[beam.pos.y][beam.pos.x].energized & bit(beam.dir)) == 0;
    }

    return false;
  };

  auto mark = [&](Beam beam) {
    assert(0 <= beam.pos.x && beam.pos.x < width && 0 <= beam.pos.y && beam.pos.y < height);
    grid[beam.pos.y][beam.pos.x].energized |= bit(beam.dir);
  };

  auto energy = [&](Beam init) {
    for (auto& line : grid) {
      for (auto& tile : line) {
        tile.energized = 0;
      }
    }

    std::cout << "Init: " << init.pos.x << ',' << init.pos.y << '\n';

    std::vector<Beam> beams;
    beams.push_back(init);

    while (!beams.empty()) {
      std::vector<Beam> next;

      for (auto beam : beams) {
        mark(beam);

        switch (grid[beam.pos.y][beam.pos.x].c) {
          case '.':
            beam.pos.x += beam.dir.x;
            beam.pos.y += beam.dir.y;

            if (valid(beam)) {
              next.push_back(beam);
            }

            break;
          case '/':
          {
            Vec new_dir;
            new_dir.x = -beam.dir.y;
            new_dir.y = -beam.dir.x;

            beam.pos.x += new_dir.x;
            beam.pos.y += new_dir.y;
            beam.dir = new_dir;

            if (valid(beam)) {
              next.push_back(beam);
            }

            break;
          }

          case '\\':
          {
            Vec new_dir;
            new_dir.x = beam.dir.y;
            new_dir.y = beam.dir.x;

            beam.pos.x += new_dir.x;
            beam.pos.y += new_dir.y;
            beam.dir = new_dir;

            if (valid(beam)) {
              next.push_back(beam);
            }

            break;
          }


          case '-':
            if (beam.dir.y == 0) {
              beam.pos.x += beam.dir.x;
              beam.pos.y += beam.dir.y;

              if (valid(beam)) {
                next.push_back(beam);
              }
            } else {
              for (int dirx : { -1, 1 }) {
                Beam other = beam;;

                other.dir.x = dirx;
                other.dir.y = 0;

                other.pos.x += other.dir.x;
                other.pos.y += other.dir.y;

                if (valid(other)) {
                  next.push_back(other);
                }
              }
            }

            break;

          case '|':
            if (beam.dir.x == 0) {
              beam.pos.x += beam.dir.x;
              beam.pos.y += beam.dir.y;

              if (valid(beam)) {
                next.push_back(beam);
              }
            } else {
              for (int diry : { -1, 1 }) {
                Beam other = beam;;

                other.dir.x = 0;
                other.dir.y = diry;

                other.pos.x += other.dir.x;
                other.pos.y += other.dir.y;

                if (valid(other)) {
                  next.push_back(other);
                }
              }
            }
            break;

          default:
            break;
        }
      }

      // std::cout << "Same: " << same << '\n';
      //
      // for (auto& line : grid) {
      //   for (auto& tile : line) {
      //     if (tile.energized != 0) {
      //       std::cout << '#';
      //     } else {
      //       std::cout << '.';
      //     }
      //   }
      //   std::cout << '\n';
      // }

      beams = std::move(next);
    }

    int count = 0;

    for (auto& line : grid) {
      for (auto& tile : line) {
        if (tile.energized != 0) {
          ++count;
          std::cout << '#';
        } else {
          std::cout << '.';
        }
      }
      std::cout << '\n';
    }

    return count;
  };

  Beam init;
  init.pos.x = 0;
  init.pos.y = 0;
  init.dir.x = 1;
  init.dir.y = 0;

  auto e = energy(init);
  std::cout << "Energy: " << e << '\n';
}
