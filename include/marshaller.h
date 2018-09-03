// marshaller.h
#pragma once

/**
 * \file \brief Marshalling operation from the Static Analysis.
 * This file provides an interface to receive information from a static analyis,
 * and outputs the graph to a format readable by the graph_parser.
 */

#include <string>
#include <unordered_map>

/**
 * These typedefs provide a loose link between the nav_graph types and the
 * analysis
 */
using USR = std::string;
using Info = std::string;
using NodeId = unsigned;
using EdgeId = std::pair<NodeId, NodeId>;

/*
 * trivial hash...
 */
namespace std {
template <> struct hash<EdgeId> {
  size_t operator()(const EdgeId &e) const {
    return std::hash<NodeId>()(e.first) +
           (std::hash<NodeId>()(e.second) << 8 * sizeof(NodeId));
  }
};
}; // namespace std

/**
 * \brief \class GraphOutputter manages information obtained from analyzing some
 * graph (i.e. AST), and outputs it in the correct format for the graph_parser
 * to parse.
 */
class Marshaller {
private:
  unsigned node_count_;
  std::unordered_map<USR, NodeId> nodes_;
  std::unordered_map<NodeId, Info> node_info_;
  std::unordered_map<EdgeId, Info> edge_info_;

  NodeId get_new_NodeId();

public:

  Marshaller();

  NodeId add_node(const USR&, const Info& info = "");
  bool check_node(const USR&) const;

  void add_edge(const USR&, const USR&, const Info& info = "");
  bool check_edge(const USR&, const USR&) const;

  std::string dump() const;
};
