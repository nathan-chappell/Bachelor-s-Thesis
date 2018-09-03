// nav_graph.cc

#include "nav_graph.h"

#include <cctype>
#include <functional>

using namespace std;

NavNode::NavNode() {}

NavNode::~NavNode() {}

NavNode::NavNode(C position, Grid grid, NodeId id, string &&info)
    : position_(position), grid_(grid), id_(id), info_(move(info)) {}

NodeId NavNode::get_Id() const { return id_; }

C NavNode::get_position() const { return position_; }
void NavNode::set_position(C position) { position_ = position; }

Grid NavNode::get_grid() const { return grid_; }
void NavNode::set_grid(Grid grid) { grid_ = grid; }

int NavNode::get_alg_data() const { return alg_data_; }
void NavNode::set_alg_data(int info) { alg_data_ = info; }

std::string NavNode::get_info() const { return info_; }

BoundingRectangle NavNode::get_bounding_rectangle() {
  return BoundingRectangle(get_position(), get_position() + C(30, 15)); // demo
}

bool NavNode::is_equal(const NodeInterface &node) {
  return id_ == dynamic_cast<const NavNode &>(node).id_;
}

string NavNode::dump() const {
  string dump = "\"node\": {";
  dump += "\"id\": " + to_string(id_);
  dump += ", \"info\": \"" + info_ + "\"";
  dump += " }";
  return dump;
}

bool UnderlyingGraph::check_NodeId(const NodeId &id) const {
  return nodes_.count(id);
}

bool UnderlyingGraph::check_Edge(const Endpoints &e) const {
  if (!edges_.count(e.l))
    return false;
  for (auto &&r : edges_.find(e.l)->second) {
    if (e.r == r) {
      return true;
    }
  }
  return false;
}

void UnderlyingGraph::add_node(NavNode &&node) {
  nodes_[node.id_] = std::move(node);
}

void UnderlyingGraph::add_edge(const Endpoints &e, EdgeInfo &&info) {
  edges_[e.l].push_back(e.r);
  edge_info_[e] = std::move(info);
}

size_t UnderlyingGraph::num_nodes() const { return nodes_.size(); }
size_t UnderlyingGraph::num_edges() const { return edges_.size(); }

EdgeInfo UnderlyingGraph::get_EdgeInfo(const Endpoints &e) const {
  if (!edge_info_.count(e))
    return EdgeInfo("");
  return edge_info_.find(e)->second;
}

// assumed that the information exists
string dump_edge(const UnderlyingGraph &g, const Endpoints &e) {
  return "\"endpoints\": " + e.dump() +
         ", \"info\": " + g.get_EdgeInfo(e).dump();
}

string UnderlyingGraph::dump() const {
  string dump;

  for (auto &&i = nodes_.begin(); i != nodes_.end(); ++i) {
    dump += "\"node\": {" + i->second.dump() + "}";
    if (next(i) != nodes_.end()) {
      dump += ",";
    }
    dump += "\n";
  }

  for (auto &&l_i = edges_.begin(); l_i != edges_.end(); ++l_i) {
    for (auto &&r_i = l_i->second.begin(); r_i != l_i->second.end(); ++r_i) {

      dump +=
          "\"edge\": {" + dump_edge(*this, Endpoints(l_i->first, *r_i)) + "}";
      if (next(l_i) != edges_.end() || (next(r_i) != l_i->second.end())) {
        dump += ",";
      }
      dump += "\n";
    }
  }

  return dump;
}
