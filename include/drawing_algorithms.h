// drawing_algorithms.h
#pragma once

#include <list>
#include <vector>

#include "common.h"

/**
 * \brief \function DfsGridDrawing assigns positions to nodes by going through
 * the graph in a DFS manner.
 */
void DfsGridDrawing(NodeInterface& node, Grid &cur_grid);
