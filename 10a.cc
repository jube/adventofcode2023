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

}

int main() {

  std::vector<std::vector<uint8_t>> tiles;
  std::size_t height = 0;
  std::size_t width = 0;

  std::size_t xs = 0;
  std::size_t ys = 0;

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.empty() || line == "\n") {
      continue;
    }

    std::vector<uint8_t> tile_line;

    for (auto c : line) {
      if (c == 'S') {
        xs = tile_line.size();
        ys = tiles.size();
      }
      tile_line.push_back(letter_to_tile(c));
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

  if (xs > 0 && (tiles[ys][xs - 1] & Right) != 0) {
    tiles[ys][xs] |= Left;
    current_direction = Left;
  }

  if (xs < width - 1 && (tiles[ys][xs + 1] & Left) != 0) {
    tiles[ys][xs] |= Right;
    current_direction = Right;
  }

  if (ys > 0 && (tiles[ys - 1][xs] & Bottom) != 0) {
    tiles[ys][xs] |= Top;
    current_direction = Top;
  }

  if (ys < height - 1 && (tiles[ys + 1][xs] & Top) != 0) {
    tiles[ys][xs] |= Bottom;
    current_direction = Bottom;
  }

  assert(current_direction != 0);

  std::size_t x = xs;
  std::size_t y = ys;
  std::size_t length = 0;

  for (;;) {
    // std::cout << "Position: " << x << ',' << y << '\n';

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

    uint8_t tile = tiles[y][x];
    current_direction = tile & ~opposite(current_direction);
  }

  std::cout << "Length: " << length << " -> " << length / 2 << '\n';
}
