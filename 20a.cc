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

  for (auto& [ name, mod ] : system.modules) {
    switch (mod.type) {
      case ModuleType::Broadcaster:
        assert(name == "broadcaster");
        std::cout << name;
        break;
      case ModuleType::FlipFlop:
        std::cout << '%' << name;
        break;
      case ModuleType::Conjunction:
        std::cout << '&' << name;
        break;
      default:
        continue;
    }

    std::cout << " -> ";

    for (auto& destination : mod.destinations) {
      std::cout << destination << ", ";
    }

    std::cout << '\n';
  }

  // init conjunction modules

  for (auto& [ name, mod ] : system.modules) {
    for (auto& destination : mod.destinations) {
      if (system.modules[destination].type == ModuleType::Conjunction) {
        system.modules[destination].conjunction_state.emplace(name, Pulse::Low);
      }
    }
  }

  std::cout << "=======\n";

  long lo_count = 0;
  long hi_count = 0;

  for (int i = 0; i < 1000; ++i) {

    std::queue<std::tuple<std::string, Pulse, std::string>> q;
    q.push(std::make_tuple("button", Pulse::Low, "broadcaster"));

    while (!q.empty()) {
      auto [ from, pulse, name ] = q.front();
      q.pop();

      std::cout << from << " -" << (pulse == Pulse::Low ? "low" : "high") << "-> " << name << '\n';

      if (pulse == Pulse::Low) {
        ++lo_count;
      } else {
        ++hi_count;
      }

      assert(system.modules.find(name) != system.modules.end());
      auto& mod = system.modules[name];

      switch (mod.type) {
        case ModuleType::Broadcaster:
          for (auto& destination : mod.destinations) {
            q.push(std::make_tuple(name, pulse, destination));
          }
          break;

        case ModuleType::FlipFlop:
          if (pulse == Pulse::Low) {
            if (mod.flip_flop_state == FlipFlopState::Off) {
              mod.flip_flop_state = FlipFlopState::On;

              for (auto& destination : mod.destinations) {
                q.push(std::make_tuple(name, Pulse::High, destination));
              }
            } else {
              mod.flip_flop_state = FlipFlopState::Off;

              for (auto& destination : mod.destinations) {
                q.push(std::make_tuple(name, Pulse::Low, destination));
              }
            }
          }
          break;

        case ModuleType::Conjunction:
          mod.conjunction_state[from] = pulse;

          if (std::all_of(mod.conjunction_state.begin(), mod.conjunction_state.end(), [](auto& pair) { return pair.second == Pulse::High; })) {
            for (auto& destination : mod.destinations) {
              q.push(std::make_tuple(name, Pulse::Low, destination));
            }
          } else {
            for (auto& destination : mod.destinations) {
              q.push(std::make_tuple(name, Pulse::High, destination));
            }
          }
          break;

        case ModuleType::Untyped:
          break;
      }

    }

    std::cout << "------\n";

  }

  std::cout << "Product: " << lo_count * hi_count << '\n';
}
