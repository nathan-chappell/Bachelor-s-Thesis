// nav_graph.h
#pragma once

/**
 * \file \brief This file contains header information necessary to use the
 * nav_graph
 *
 * The nav_graph is the underlying structure used for the program to view,
 * browse, and manipulate relations.
 *
 */

#include "common_graph.h"

#include <list>
#include <string>
#include <unordered_map>

class NavGraph;

namespace std {
template <> struct hash<Endpoints> {
  size_t operator()(const Endpoints &e) const {
    return std::hash<NodeId>()(e.l) +
           (std::hash<NodeId>()(e.r) << 8 * sizeof(NodeId));
  }
};
}; // namespace std

/**
 * \brief \class NavNode is the fundamental node type which the graph shall
 * maintain.
 *
 */
class NavNode : public NodeInterface {
private:
  C position_;
  Grid grid_;
  /** \brief info_ is used by algorithms */
  int alg_data_;
  /** \brief id_ is a unique id used for indexing nodes in the nav_graph */
  NodeId id_;
  /** \brief info describes the node for display */
  std::string info_;

  std::list<NodeId> neighbours_;

public:
  NavNode(C, Grid, NodeId, std::string&&);
  NavNode();

  NodeId get_Id() const override;

  C get_position() const override;
  void set_position(C) override;

  Grid get_grid() const override;
  void set_grid(Grid) override;

  int get_alg_data() const override;
  void set_alg_data(int) override;

  std::string get_info() const override;

  BoundingRectangle get_bounding_rectangle() override;
  bool is_equal(const NodeInterface &) override;

  std::string dump() const;

  ~NavNode() override;

  friend class UnderlyingGraph;
};

/**
 * \brief \class NavGraph is the central info structure to the whole damn thing
 */
class UnderlyingGraph {
private:
  std::unordered_map<NodeId, NavNode> nodes_;
  std::unordered_map<NodeId, std::list<NodeId>> edges_;
  std::unordered_map<Endpoints,EdgeInfo> edge_info_;

public:
  bool check_NodeId(const NodeId &id) const;
  bool check_Edge(const Endpoints &e) const;
  void add_node(NavNode&& node);
  void add_edge(const Endpoints &e, EdgeInfo&& info);

  size_t num_nodes() const;
  size_t num_edges() const;

  EdgeInfo get_EdgeInfo(const Endpoints &e) const;

  std::string dump() const;
};
