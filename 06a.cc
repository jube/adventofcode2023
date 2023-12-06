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

  constexpr int Time[] = { 61, 70, 90, 66 };
  constexpr int Distance[] = { 643, 1184, 1362, 1041 };

  int product = 1;

  for (std::size_t i = 0; i < std::size(Time); ++i) {
    const int time = Time[i];
    const int distance = Distance[i];

    int count = 0;

    for (int speed = 1; speed < time; ++speed) {
      int race = (time - speed) * speed;

      if (race > distance) {
        ++count;
      }

    }

    product *= count;
  }

  std::cout << product << '\n';
}
