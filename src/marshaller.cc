// marshaller.cc

#include "marshaller.h"

using namespace std;

Marshaller::Marshaller() : node_count_(0) {}

NodeId Marshaller::get_new_NodeId() { return node_count_++; }

NodeId Marshaller::add_node(const USR &usr, const Info &info) {
  NodeId id;
  if (check_node(usr)) {
    id = nodes_[usr];
  } else {
    id = get_new_NodeId();
  }

  nodes_[usr] = id;
  node_info_[id] += info;

  return id;
}

bool Marshaller::check_node(const USR &usr) const { return nodes_.count(usr); }

void Marshaller::add_edge(const USR &l, const USR &r, const Info &info) {
  NodeId lid;
  NodeId rid;

  if (check_node(l)) {
    lid = nodes_[l];
  } else {
    lid = add_node(l);
  }

  if (check_node(r)) {
    rid = nodes_[r];
  } else {
    rid = add_node(r);
  }

  edge_info_[make_pair(lid,rid)] += info;
}

bool Marshaller::check_edge(const USR &l, const USR &r) const {
  auto lid_it = nodes_.find(l);
  auto rid_it = nodes_.find(r);

  if (lid_it == nodes_.end() || rid_it == nodes_.end()) {
    return false;
  } else {
    return edge_info_.count(make_pair(lid_it->second, rid_it->second));
  }
}

string Marshaller::dump() const {
  string result;
  const string indent = "  ";
  bool have_edges = edge_info_.size() > 0;

  result += "{\n" + indent + "\"graph\": {\n";
  for (auto &&it = nodes_.begin(); it != nodes_.end(); ++it) {
    result += indent + indent +
              "\"node\": { \"id\": " + ::to_string(it->second) +
              ", \"info\": \"" + node_info_.find(it->second)->second + "\" }";
    if (have_edges || next(it) != nodes_.end()) {
      result += ",";
    }
    result += "\n";
  }

  for (auto &&it = edge_info_.begin(); it != edge_info_.end(); ++it) {
    result += indent + indent + "\"edge\": { \"endpoints\": [" +
              ::to_string(it->first.first) + "," +
              ::to_string(it->first.second) + "], \"info\": \"" + it->second +
              "\" }";

    if (next(it) != edge_info_.end()) {
      result += ",";
    }
    result += "\n";
  }

  result += indent + "}\n}";

  return result;
}
