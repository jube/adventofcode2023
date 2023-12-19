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

  struct Range {
    std::string current = "in";
    int xmin = 1;
    int xmax = 4000;
    int mmin = 1;
    int mmax = 4000;
    int amin = 1;
    int amax = 4000;
    int smin = 1;
    int smax = 4000;
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

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.empty() || line == "\n") {
      break;
    }
  }

  std::vector<Range> accepted;
  std::vector<Range> current_ranges;
  Range init;

  current_ranges.push_back(init);

  while (!current_ranges.empty()) {
    std::vector<Range> next_ranges;

    std::cout << "-------- " << current_ranges.size() << '\n';

    for (auto range : current_ranges) {
      std::cout << "x [" << range.xmin << ',' << range.xmax << "], m [" << range.mmin << ',' << range.mmax << "], a [" << range.amin << ',' << range.amax << "], s [" << range.smin << ',' << range.smax << "] : " << range.current << '\n';
    }

    std::cout << "----\n";

    for (auto range : current_ranges) {
      // make one transition

      std::cout << "Current range: x [" << range.xmin << ',' << range.xmax << "], m [" << range.mmin << ',' << range.mmax << "], a [" << range.amin << ',' << range.amax << "], s [" << range.smin << ',' << range.smax << "] : " << range.current << '\n';

      auto accept_or_next = [&](const Range& range) {
        if (range.current == "A") {
          accepted.push_back(range);
          std::cout << " = Accepted\n";
        } else if (range.current != "R") {
          next_ranges.push_back(range);
          std::cout << " = Next\n";
        } else {
          std::cout << " = Rejected\n";
        }
      };

      auto iterator = workflows.find(range.current);
      assert(iterator != workflows.end());

      auto& [ name, workflow ] = *iterator;

      for (auto& transition : workflow.transitions) {
        if (transition.part == NoPart) {
          std::cout << "End of workflow: " << transition.target << '\n';
          range.current = transition.target;
          std::cout << "Range";
          accept_or_next(range);
          break;
        }

        int Range::* min = nullptr;
        int Range::* max = nullptr;

        switch (transition.part) {
          case 'x':
            min = &Range::xmin;
            max = &Range::xmax;
            break;
          case 'm':
            min = &Range::mmin;
            max = &Range::mmax;
            break;
          case 'a':
            min = &Range::amin;
            max = &Range::amax;
            break;
          case 's':
            min = &Range::smin;
            max = &Range::smax;
            break;
          default: assert(false); break;
        }

        std::cout << "Part: " << transition.part << " in [" << range.*min << ',' << range.*max << "]\n";
        std::cout << "Cond: " << transition.condition << ' ' << transition.value << '\n';

        auto test_condition = [&](int value) {
          if (transition.condition == '<') {
            return value < transition.value;
          }

          if (transition.condition == '>') {
            return value > transition.value;
          }

          assert(false);
          return false;
        };


        if (range.*min < transition.value && transition.value < range.*max) {
          std::cout << "+ Split in two...\n";
          // split in two
          Range left = range;
          Range right = range;

          left.*max = std::min(left.*max, transition.value);
          right.*min = std::max(right.*min, transition.value);

          if (test_condition(range.*min)) {
            assert(!test_condition(range.*max));
            // left goes next
            left.current = transition.target;
            --(left.*max);
            std::cout << "-> L (" << left.*min << ',' << left.*max << ")";
            accept_or_next(left);

            // keep right
            std::cout << "-> R (" << right.*min << ',' << right.*max << ") is kept\n";
            range = right;
          } else {
            assert(test_condition(range.*max));
            // right goes next
            right.current = transition.target;
            ++(right.*min);
            std::cout << "-> R (" << right.*min << ',' << right.*max << ")";
            accept_or_next(right);

            // keep left
            std::cout << "-> L (" << left.*min << ',' << left.*max << ") is kept\n";
            range = left;
          }
        } else if (transition.value <= range.*min) {
          std::cout << "+ Value at the left...\n";

          if (test_condition(range.*min)) {
            // range goes in the next state as a whole
            std::cout << "Whole range";
            range.current = transition.target;
            accept_or_next(range);
            break;
          }
        } else if (transition.value >= range.*max) {
          std::cout << "+ Value at the right...\n";

          if (test_condition(range.*max)) {
            // range goes in the next state as a whole
            range.current = transition.target;
            std::cout << "Whole range";
            accept_or_next(range);
            break;
          }
        } else {
          assert(false);
        }

        if (range.current == "A" || range.current == "R") {
          std::cout << "Early exit";
          accept_or_next(range);
          break;
        }
      }

    }

    current_ranges = next_ranges;
  }

  long sum = 0;

  for (auto& range : accepted) {
    long all = (range.xmax - range.xmin + 1);
    all *= (range.mmax - range.mmin + 1);
    all *= (range.amax - range.amin + 1);
    all *= (range.smax - range.smin + 1);

    sum += all;
  }

  std::cout << sum << '\n';
}
