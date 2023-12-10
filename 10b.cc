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

  constexpr uint8_t None    = 0x00;
  constexpr uint8_t Top     = 0x01;
  constexpr uint8_t Right   = 0x02;
  constexpr uint8_t Bottom  = 0x04;
  constexpr uint8_t Left    = 0x08;
  constexpr uint8_t Start   = 0x10;

  uint8_t opposite(uint8_t direction) {
    switch (direction) {
      case Top:
        return Bottom;
      case Right:
        return Left;
      case Bottom:
        return Top;
      case Left:
        return Right;
      default:
        assert(false);
        break;
    }

    return None;
  }

  constexpr uint8_t Tile_F  = Bottom | Right;
  constexpr uint8_t Tile_7  = Bottom | Left;
  constexpr uint8_t Tile_J  = Top | Left;
  constexpr uint8_t Tile_L  = Top | Right;
  constexpr uint8_t Tile_H  = Right | Left;
  constexpr uint8_t Tile_V  = Top | Bottom;

  uint8_t letter_to_tile(char c) {
    switch (c) {
      case '.': return None;
      case 'S': return Start;
      case 'F': return Tile_F;
      case '7': return Tile_7;
      case 'J': return Tile_J;
      case 'L': return Tile_L;
      case '-': return Tile_H;
      case '|': return Tile_V;
      default:
        assert(false);
    }

    return 0;
  }

  constexpr uint8_t InTheLoop = 0x01;
  constexpr uint8_t LeftOfPath = 0x02;
  constexpr uint8_t RightOfPath = 0x04;

  struct Tile {
    uint8_t base = 0;
    uint8_t status = 0;
  };

  void explore(std::vector<std::vector<Tile>>& tiles, std::size_t x, std::size_t y, uint8_t status) {
    tiles[y][x].status = status;

    if (x > 0 && tiles[y][x - 1].status == None) {
      explore(tiles, x - 1, y, status);
    }

    if (x < tiles.front().size() - 1 && tiles[y][x + 1].status == None) {
      explore(tiles, x + 1, y, status);
    }

    if (y > 0 && tiles[y - 1][x].status == None) {
      explore(tiles, x, y - 1, status);
    }

    if (y < tiles.size() - 1 && tiles[y + 1][x].status == None) {
      explore(tiles, x, y + 1, status);
    }
  }


}

int main() {

  std::vector<std::vector<Tile>> tiles;
  std::size_t height = 0;
  std::size_t width = 0;

  std::size_t xs = 0;
  std::size_t ys = 0;

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.empty() || line == "\n") {
      continue;
    }

    std::vector<Tile> tile_line;

    for (auto c : line) {
      if (c == 'S') {
        xs = tile_line.size();
        ys = tiles.size();
      }
      tile_line.push_back(Tile{ letter_to_tile(c), None });
    }

    if (width == 0) {
      width = tile_line.size();
    } else {
      assert(width == tile_line.size());
    }

    tiles.emplace_back(std::move(tile_line));
    ++height;
  }

  assert(height == tiles.size());

  std::cout << "Start: " << xs << ',' << ys << '\n';

  uint8_t current_direction = 0;

  if (xs > 0 && (tiles[ys][xs - 1].base & Right) != 0) {
    tiles[ys][xs].base |= Left;
    current_direction = Left;
  }

  if (xs < width - 1 && (tiles[ys][xs + 1].base & Left) != 0) {
    tiles[ys][xs].base |= Right;
    current_direction = Right;
  }

  if (ys > 0 && (tiles[ys - 1][xs].base & Bottom) != 0) {
    tiles[ys][xs].base |= Top;
    current_direction = Top;
  }

  if (ys < height - 1 && (tiles[ys + 1][xs].base & Top) != 0) {
    tiles[ys][xs].base |= Bottom;
    current_direction = Bottom;
  }

  assert(current_direction != 0);

  std::size_t x = xs;
  std::size_t y = ys;
  std::size_t length = 0;

  for (;;) {
    // std::cout << "Position: " << x << ',' << y << '\n';

    tiles[y][x].status = InTheLoop;

    switch (tiles[y][x].base & ~Start) {
      case Tile_H:
        assert(current_direction == Right || current_direction == Left);

        if (y > 0 && tiles[y - 1][x].status == None) {
          tiles[y - 1][x].status = current_direction == Right ? LeftOfPath : RightOfPath;
        }
        if (y < height - 1 && tiles[y + 1][x].status == None) {
          tiles[y + 1][x].status = current_direction == Right ? RightOfPath : LeftOfPath;
        }
        break;
      case Tile_V:
        assert(current_direction == Bottom || current_direction == Top);

        if (x > 0 && tiles[y][x - 1].status == None) {
          tiles[y][x - 1].status = current_direction == Top ? LeftOfPath : RightOfPath;
        }
        if (x < width - 1 && tiles[y][x + 1].status == None) {
          tiles[y][x + 1].status = current_direction == Top ? RightOfPath : LeftOfPath;
        }
        break;
      case Tile_7:
      {
        assert(current_direction == Bottom || current_direction == Left);
        uint8_t status = current_direction == Bottom ? LeftOfPath : RightOfPath;

        if (y > 0 && tiles[y - 1][x].status == None) {
          tiles[y - 1][x].status = status;
        }

        if (x < width - 1 && tiles[y][x + 1].status == None) {
          tiles[y][x + 1].status = status;
        }

        if (y > 0 && x < width - 1 && tiles[y - 1][x + 1].status == None) {
          tiles[y - 1][x + 1].status = status;
        }

        break;
      }

      case Tile_L:
      {
        assert(current_direction == Right || current_direction == Top);
        uint8_t status = current_direction == Top ? LeftOfPath : RightOfPath;

        if (y < height - 1 && tiles[y + 1][x].status == None) {
          tiles[y + 1][x].status = status;
        }

        if (x > 0 && tiles[y][x - 1].status == None) {
          tiles[y][x - 1].status = status;
        }

        if (y < height - 1 && x > 0 && tiles[y + 1][x - 1].status == None) {
          tiles[y + 1][x - 1].status = status;
        }

        break;
      }

      case Tile_F:
      {
        assert(current_direction == Right || current_direction == Bottom);
        uint8_t status = current_direction == Right ? LeftOfPath : RightOfPath;

        if (y > 0 && tiles[y - 1][x].status == None) {
          tiles[y - 1][x].status = status;
        }

        if (x > 0 && tiles[y][x - 1].status == None) {
          tiles[y][x - 1].status = status;
        }

        if (y > 0 && x > 0 && tiles[y - 1][x - 1].status == None) {
          tiles[y - 1][x - 1].status = status;
        }

        break;
      }

      case Tile_J:
      {
        assert(current_direction == Left || current_direction == Top);
        uint8_t status = current_direction == Left ? LeftOfPath : RightOfPath;

        if (y < height - 1 && tiles[y + 1][x].status == None) {
          tiles[y + 1][x].status = status;
        }

        if (x < width - 1 && tiles[y][x + 1].status == None) {
          tiles[y][x + 1].status = status;
        }

        if (y < height - 1 && x < width - 1 && tiles[y + 1][x + 1].status == None) {
          tiles[y + 1][x + 1].status = status;
        }

        break;
      }

    }


    switch (current_direction) {
      case Top:
        --y;
        break;
      case Right:
        ++x;
        break;
      case Bottom:
        ++y;
        break;
      case Left:
        --x;
        break;
      default:
        assert(false);
        break;
    }

    ++length;

    assert(x < width);
    assert(y < height);

    if (x == xs && y == ys) {
      break;
    }

    uint8_t tile = tiles[y][x].base;
    current_direction = tile & ~opposite(current_direction);
  }

  for (y = 0; y < height; ++y) {
    for (x = 0; x < width; ++x) {
      auto status = tiles[y][x].status;

      if (status == LeftOfPath || status == RightOfPath) {
        explore(tiles, x, y , status);
      }
    }
  }

  std::size_t count_left = 0;
  std::size_t count_right = 0;

  for (auto& tile_line : tiles) {
    for (auto& tile : tile_line) {
      switch (tile.status) {
        case None:
          std::cout << ' ';
          break;
        case InTheLoop:
          std::cout << '.';
          break;
        case LeftOfPath:
          std::cout << 'L';
          ++count_left;
          break;
        case RightOfPath:
          std::cout << 'R';
          ++count_right;
          break;
        default:
          assert(false);
          break;
      }
    }

    std::cout << '\n';
  }


  std::cout << "Count of L: " << count_left << '\n';
  std::cout << "Count of R: " << count_right << '\n';
}
