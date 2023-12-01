#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

int main() {

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.empty() || line == "\n") {
      continue;
    }
  }

}
