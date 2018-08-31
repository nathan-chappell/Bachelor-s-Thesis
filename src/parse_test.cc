//parse_test.cc

#include "nav_graph.h"
#include "parse.h"

int main() {
  nav_graph g = std::move(parse_file("test.graph"));
  g.dump();
}
