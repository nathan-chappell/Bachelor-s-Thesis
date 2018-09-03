// graph_parser.cc

#include "graph_parser.h"

using namespace std;
using namespace parse_functions;

Endpoints eat_Endpoints(const std::string &source, parse_state &state) {
  unsigned l = 0;
  unsigned r = 0;
  check_for_char(source, state, '[');
  eat_ws(source, state);
  l = eat_unsigned(source, state);
  eat_ws(source, state);
  check_for_char(source, state, ',');
  eat_ws(source, state);
  r = eat_unsigned(source, state);
  eat_ws(source, state);
  check_for_char(source, state, ']');
  return Endpoints(l, r);
}

NavNode parse_NavNode(const string &source, parse_state &state) {
  C position;
  Grid grid;
  NodeId id;
  string info;
  string attribute;

  bool have_id = false;
  bool have_info = false;

  while (true) {
    eat_ws(source, state);
    attribute = eat_simple_attribute_declaration(source, state);
    eat_ws(source, state);
    if (attribute == "id") {
      id = eat_unsigned(source, state);
      have_id = true;
    } else if (attribute == "info") {
      info = eat_escaped_string(source, state);
      have_info = true;
    } else if (attribute == "position") {
      position = eat_C(source, state);
    } else if (attribute == "grid") {
      grid = eat_Grid(source, state);
    } else {
      parse_error("node attribute", state);
    }

    if (state.error) {
      *state.error_log << "error parsing NavNode: " << state.position() << endl;
      break;
    } else if (state.end) {
      *state.error_log << "end of file encountered while parsing NavNode at: "
                       << state.position() << endl;
      break;
    }

    eat_ws(source, state);

    if (peak_for(source, state, ',')) {
      advance_state(source, state);
      continue;
    } else if (peak_for(source, state, '}')) {
      advance_state(source, state);
      break;
    } else {
      state.error = true;
      break;
    }
  }

  if (!have_id) {
    *state.error_log << "Warning: no id read at: " << state.position() << endl;
  }

  if (!have_info) {
    *state.error_log << "Warning: no info read at: " << state.position()
                     << endl;
  }

  return NavNode(position, grid, id, std::move(info));
}

pair<Endpoints, EdgeInfo> parse_Edge(const string &source, parse_state &state) {
  Endpoints endpoints;
  EdgeInfo edgeInfo;
  string info;
  string attribute;

  bool have_endpoints = false;
  bool have_info = false;

  while (true) {
    eat_ws(source, state);
    attribute = eat_simple_attribute_declaration(source, state);
    eat_ws(source, state);
    if (attribute == "endpoints") {
      endpoints = eat_Endpoints(source, state);
      have_endpoints = true;
    } else if (attribute == "info") {
      info = eat_escaped_string(source, state);
      have_info = true;
    } else {
      parse_error("edge attribute", state);
    }

    if (state.error) {
      *state.error_log << "error parsing Edge: " << state.position() << endl;
      break;
    } else if (state.end) {
      *state.error_log << "end of file encountered while parsing Edge at: "
                       << state.position() << endl;
      break;
    }

    eat_ws(source, state);

    if (peak_for(source, state, ',')) {
      advance_state(source, state);
      continue;
    } else if (peak_for(source, state, '}')) {
      advance_state(source, state);
      break;
    } else {
      state.error = true;
      break;
    }
  }

  if (!have_endpoints) {
    *state.error_log << "Warning: no endpoints read at: " << state.position()
                     << endl;
  }

  if (!have_info) {
    *state.error_log << "Warning: no info read at: " << state.position()
                     << endl;
  }

  edgeInfo.info = move(info);

  return make_pair<Endpoints, EdgeInfo>(move(endpoints), move(edgeInfo));
}

UnderlyingGraph parse_UnderlyingGraph_Data(const string &source,
                                           parse_state &state) {
  UnderlyingGraph graph;
  NavNode curNode;
  pair<Endpoints, EdgeInfo> curEdge;
  string attribute;

  while (true) {
    eat_ws(source, state);
    attribute = eat_compound_attribute_declaration(source, state);
    eat_ws(source, state);
    if (attribute == "node") {
      curNode = parse_NavNode(source, state);
      if (graph.check_NodeId(curNode.get_Id())) {
        *state.error_log << "Redeclared Node: " << to_string(curNode.get_Id())
                         << ", at: " << state.position() << endl;
        state.error = true;
      } else {
        graph.add_node(move(curNode));
      }
    } else if (attribute == "edge") {
      curEdge = parse_Edge(source, state);
      if (graph.check_Edge(curEdge.first)) {
        *state.error_log << "Redeclared Edge: " << curEdge.first.dump()
                         << ", at: " << state.position() << endl;
        state.error = true;
      } else {
        graph.add_edge(curEdge.first, move(curEdge.second));
      }
    } else {
      parse_error("UnderlyingGraph attribute", state);
    }

    if (state.error) {
      *state.error_log << "error parsing UnderlyingGraph: " << state.position()
                       << endl;
      break;
    } else if (state.end) {
      *state.error_log
          << "end of file encountered while parsing UnderlyingGraph at: "
          << state.position() << endl;
      break;
    }

    eat_ws(source, state);

    if (peak_for(source, state, ',')) {
      advance_state(source, state);
      continue;
    } else if (peak_for(source, state, '}')) {
      advance_state(source, state);
      break;
    } else {
      state.error = true;
      break;
    }
  }

  if (graph.num_nodes() == 0) {
    *state.error_log << "Warning: no nodes read at: " << state.position()
                     << endl;
  }

  if (graph.num_edges() == 0) {
    *state.error_log << "Warning: no edges read at: " << state.position()
                     << endl;
  }

  return graph;
}

string get_file(const string &fname) {
  string file_string;
  string line;
  ifstream file(fname);
  while (file.good()) {
    getline(file, line);
    file_string += line + "\n";
  }
  return file_string;
}

/*
 * For now, we'll just parse this damn thing from a file, when the UI/tree-view
 * starts to
 * grow wings we'll have to parse views and who knows what, but this is a good
 * start.
 */

UnderlyingGraph parse_UnderlyingGraph(const string &source, parse_state &state) {
  UnderlyingGraph graph;
  string attribute;

  eat_ws(source, state);
  check_for_char(source, state, '{');
  eat_ws(source, state);

  attribute = eat_compound_attribute_declaration(source, state);
  if (attribute != "graph") {
    parse_error("graph", state);
  }

  eat_ws(source, state);
  graph = parse_UnderlyingGraph_Data(source, state);
  eat_ws(source, state);
  check_for_char(source, state, '}');
  eat_ws(source, state);

  if (state.error) {
    *state.error_log << "error parsing UnderlyingGraphFromFile: "
                     << state.position() << endl;
  } else if (!state.end) {
    parse_error("end of file", state);
  }

  return move(graph);
}

UnderlyingGraph parse_UnderlyingGraphFromFile(const string &fname) {
  parse_state dummy;
  return parse_UnderlyingGraphFromFile(fname, dummy);
}

UnderlyingGraph parse_UnderlyingGraphFromFile(const string &fname,
                                              parse_state &state) {
  string source = get_file(fname);
  return parse_UnderlyingGraph(source, state);
}
