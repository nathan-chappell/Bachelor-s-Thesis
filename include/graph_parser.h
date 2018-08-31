// graph_parser.h
#pragma once

#include "common.h"
#include "nav_graph.h"
#include "parse_functions.h"

#include <fstream>

Endpoints eat_Endpoints(const std::string &source,
                        parse_functions::parse_state &state);

NavNode parse_NavNode(const std::string &source,
                      parse_functions::parse_state &state);

std::pair<Endpoints, EdgeInfo> parse_Edge(const std::string &source,
                                          parse_functions::parse_state &state);

UnderlyingGraph parse_UnderlyingGraph(const std::string &source,
                                      parse_functions::parse_state &state);

UnderlyingGraph
parse_UnderlyingGraphFromFile(const std::string &fname);

UnderlyingGraph
parse_UnderlyingGraphFromFile(const std::string &fname, parse_functions::parse_state&);
