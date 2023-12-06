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

  const long time = 61709066;
  const long distance = 643118413621041;

  long count = 0;

  for (int speed = 1; speed < time; ++speed) {
    long race = (time - speed) * speed;

    if (race > distance) {
      ++count;
    }

  }

  std::cout << count << '\n';
}
