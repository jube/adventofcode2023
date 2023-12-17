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

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

namespace {

  struct Point {};
  struct Route {
    int heat = 0;
  };

  using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, Point, Route>;

}

int main() {

  std::vector<std::vector<int>> raw;

  for (std::string line; std::getline(std::cin, line); ) {
    if (line.empty() || line == "\n") {
      continue;
    }

    std::vector<int> raw_line;

    for (auto c : line) {
      raw_line.push_back(c - '0');
    }

    raw.push_back(std::move(raw_line));
  }

  const std::size_t width = raw.front().size();
  const std::size_t height = raw.size();


  std::size_t count = 2 * width * height;

  Graph graph;

  std::vector<Graph::vertex_descriptor> vertices;

  for (std::size_t i = 0; i < count; ++i) {
    vertices.push_back(boost::add_vertex(graph));
  }

  assert(vertices.size() == count);

  std::size_t start = vertices.size();
  vertices.push_back(boost::add_vertex(graph));
  std::size_t end = vertices.size();
  vertices.push_back(boost::add_vertex(graph));

  auto vertex_index = [&](std::size_t x, std::size_t y, std::size_t level) -> std::size_t {
    assert(x < width);
    assert(y < height);
    assert(level < 2);

    std::size_t index = level * width * height + y * width + x;
    return index;
  };

  auto vertex = [&](std::size_t x, std::size_t y, std::size_t level) -> Graph::vertex_descriptor {
    std::size_t index = vertex_index(x, y, level);
    assert(index < vertices.size());

    return vertices[index];
  };

  for (std::size_t j = 0; j < height; ++j) {
    for (std::size_t i = 0; i < width; ++i) {
      int heat_left = 0;
      int heat_right = 0;
      int heat_up = 0;
      int heat_down = 0;

      for (std::size_t k = 1; k <= 3; ++k) {
        if (i >= k) {
          heat_left += raw[j][i - k];
        }

        if (i + k < width) {
          heat_right += raw[j][i + k];
        }

        if (j >= k) {
          heat_up += raw[j - k][i];
        }

        if (j + k < height) {
          heat_down += raw[j + k][i];
        }
      }

      for (std::size_t k = 4; k <= 10; ++k) {
        if (i >= k) {
          heat_left += raw[j][i - k];
          auto [ e, inserted ] = boost::add_edge(vertex(i, j, 0), vertex(i - k, j, 1), graph);
          graph[e].heat = heat_left;
        }

        if (i + k < width) {
          heat_right += raw[j][i + k];
          auto [ e, inserted ] = boost::add_edge(vertex(i, j, 0), vertex(i + k, j, 1), graph);
          graph[e].heat = heat_right;
        }

        if (j >= k) {
          heat_up += raw[j - k][i];
          auto [ e, inserted ] = boost::add_edge(vertex(i, j, 1), vertex(i, j - k, 0), graph);
          graph[e].heat = heat_up;
        }

        if (j + k < height) {
          heat_down += raw[j + k][i];
          auto [ e, inserted ] = boost::add_edge(vertex(i, j, 1), vertex(i, j + k, 0), graph);
          graph[e].heat = heat_down;
        }
      }
    }
  }

  boost::add_edge(vertices[start], vertex(0, 0, 0), graph);
  boost::add_edge(vertices[start], vertex(0, 0, 1), graph);

  boost::add_edge(vertex(width - 1, height - 1, 0), vertices[end], graph);
  boost::add_edge(vertex(width - 1, height - 1, 1), vertices[end], graph);

  std::vector<int> distances(boost::num_vertices(graph), 0);
  boost::dijkstra_shortest_paths(graph, vertices[start], boost::weight_map(get(&Route::heat, graph)).distance_map(make_iterator_property_map(distances.begin(),
                                               get(boost::vertex_index, graph))));


  std::cout << "Distance: " << distances.back() << '\n';
}
