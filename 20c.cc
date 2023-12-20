#include <cassert>
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
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

  enum class ModuleType {
    Untyped,
    FlipFlop,
    Conjunction,
    Broadcaster,
  };

  enum class Pulse {
    Low,
    High
  };

  enum class FlipFlopState {
    On,
    Off,
  };

  struct Module;

  struct Module {
    ModuleType type = ModuleType::Untyped;
    std::vector<std::string> destinations;
    std::map<std::string, Pulse> conjunction_state;
    FlipFlopState flip_flop_state = FlipFlopState::Off;
  };

  struct System {
    std::map<std::string, Module> modules;
  };

}

int main() {
  System system;

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.empty() || line == "\n") {
      continue;
    }

    auto parts = split_string(line, '-');
    assert(parts.size() == 2);
    std::string name(parts[0]);
    assert(name.back() == ' ');
    name.pop_back();

    if (name == "broadcaster") {
      auto [ iterator, inserted ] = system.modules.emplace(name, Module{});
      assert(inserted);
      iterator->second.type = ModuleType::Broadcaster;
    } else {
      char type = name[0];
      name = name.substr(1);
      auto [ iterator, inserted ] = system.modules.emplace(name, Module{});
      assert(inserted);

      if (type == '%') {
        iterator->second.type = ModuleType::FlipFlop;
      } else if (type == '&') {
        iterator->second.type = ModuleType::Conjunction;
      } else {
        assert(false);
      }
    }

    std::string second(parts[1].substr(1));
    auto destinations = split_string(second, ',');

    for (auto destination : destinations) {
      system.modules[name].destinations.push_back(std::string(destination.substr(1)));
    }
  }

  std::cout << "digraph day20 {\n";

  for (auto& [ name, mod ] : system.modules) {

    for (auto& destination : mod.destinations) {
      std::cout << name << " -> " << destination << '\n';
    }

  }

  std::cout << "}\n";

}
