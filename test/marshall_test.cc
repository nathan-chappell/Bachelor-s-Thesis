//marshall_test.cc

#include "marshaller.h"
#include "../include/graph_parser.h"

#include <iostream>
#include <sstream>

using namespace std;
using namespace parse_functions;

int main() {
  Marshaller m;

  m.add_node("a", "node 1");
  m.add_node("b", "node 2");
  m.add_node("c", "node 3");

  m.add_edge("a", "b", "a to b");
  m.add_edge("a", "c", "a to c");
  m.add_edge("c", "b", "c to b");

  //cout << "marshall dump:" << endl << m.dump() << endl;

  ostringstream oss;
  parse_state state(&oss);
  parse_UnderlyingGraph(m.dump(), state);
  if (state.error) {
    cout << "Error parsing marshal dump, dumping log.\n" << oss.str() << endl;
  } else {
    cout << "Marshall testing complete." << endl;
  }
}
