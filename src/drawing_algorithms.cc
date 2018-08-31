// drawing_algorithms.cc

#include "drawing_algorithms.h"

using namespace std;

namespace alg_data {
const int white = 0;
const int grey = 0;
const int black = 0;
}; // namespace alg_data

void DfsGridDrawing(NodeInterface &node, Grid &cur_grid) {
  /*
   * could set to black, but later it may be useful to know which nodes are
   * being processed and which are done.
   */
  node.set_data(alg_data::grey);
  node.set_grid(cur_grid);

  // recurse, but increase depth (x)
  cur_grid += C(1,0);
  for (auto &&n : node.get_neighbours()) {
    if (n->get_data() == alg_data::white) {
      DfsGridDrawing(cur_grid);
    }
    //the increasing y coordinate accumulates this branch's width
    cur_grid += C(0, 1);
  }
  // return to current depth
  cur_grid -= C(1,0);

  node.set_data(alg_data::black);
}
