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

  constexpr char NoPart = '\0';

  struct Transition {
    char part = NoPart;
    char condition = '<';
    int value = 0;
    std::string target;
  };

  struct Workflow {
    std::vector<Transition> transitions;
  };

  struct System {
    int x = 0;
    int m = 0;
    int a = 0;
    int s = 0;
  };

}

int main() {
  std::map<std::string, Workflow> workflows;

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.empty() || line == "\n") {
      break;
    }

    Workflow workflow;

    auto start = line.find('{');
    assert(start != std::string_view::npos);

    std::string name = line.substr(0, start);

    auto end = line.find('}');
    auto all_conditions = line.substr(start + 1, end - start - 1);
    auto raw_conditions = split_string(all_conditions, ',');

    for (auto raw_condition : raw_conditions) {
      // std::cout << raw_condition << '\n';

      if (auto colon = raw_condition.find(':'); colon != std::string_view::npos) {
        Transition transition;

        transition.part = raw_condition[0];
        transition.condition = raw_condition[1];
        transition.value = std::stoi(std::string(raw_condition.substr(2, colon - 2)));
        transition.target = raw_condition.substr(colon + 1);

        // std::cout << transition.part << transition.condition << transition.value << ':' << transition.target << '\n';

        workflow.transitions.push_back(std::move(transition));
      } else {
        Transition transition;
        transition.target = raw_condition;

        // std::cout << transition.target << '\n';

        workflow.transitions.push_back(std::move(transition));
      }

    }

    workflows[name] = std::move(workflow);
  }

  std::vector<System> systems;

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.empty() || line == "\n") {
      break;
    }

    // std::cout << line << '\n';

    line = line.substr(1, line.size() - 2);
    auto parts = split_string(line, ',');
    assert(parts.size() == 4);


    System system;

    for (auto part : parts) {
      switch (part[0]) {
        case 'x':
          system.x = std::stoi(std::string(part.substr(2)));
          break;
        case 'm':
          system.m = std::stoi(std::string(part.substr(2)));
          break;
        case 'a':
          system.a = std::stoi(std::string(part.substr(2)));
          break;
        case 's':
          system.s = std::stoi(std::string(part.substr(2)));
          break;
      }
    }

    // std::cout << "{x=" << system.x << ",m=" << system.m << ",a=" << system.a << ",s=" << system.s << "}\n";

    systems.push_back(std::move(system));
  }


  int sum = 0;

  for (auto& system : systems) {
    std::string current = "in";

    for (;;) {
      std::cout << current << " -> ";

      auto iterator = workflows.find(current);
      assert(iterator != workflows.end());

      auto& [ name, workflow ] = *iterator;

      for (auto& transition : workflow.transitions) {
        if (transition.part == NoPart) {
          current = transition.target;
          break;
        }

        int part = 0;

        switch (transition.part) {
          case 'x': part = system.x; break;
          case 'm': part = system.m; break;
          case 'a': part = system.a; break;
          case 's': part = system.s; break;
          default: assert(false); break;
        }

        bool condition = true;

        if (transition.condition == '<') {
          condition = part < transition.value;
        } else if (transition.condition == '>') {
          condition = part > transition.value;
        } else {
          assert(false);
        }

        if (condition) {
          current = transition.target;
          break;
        }
      }

      if (current == "A" || current == "R") {
        break;
      }
    }

    std::cout << current << '\n';

    if (current == "A") {
      sum += system.x + system.m + system.a + system.s;
    }
  }

  std::cout << "Sum: " << sum << '\n';

}
