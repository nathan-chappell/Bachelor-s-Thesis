// common.h
#pragma once

#include <complex>
#include <functional>
#include <limits>
#include <list>
#include <string>

/*
 * Note, complex numbers will be used to represent grid coordinates.  In this
 * case, the real component corresponds to the x component and the imaginary to
 * the y.  In gtkmm, the y component is reversed, so increasing a shape's y
 * coordinates will move it DOWNWARD.
 */

using C = std::complex<double>;
using Grid = std::complex<int>;
using NodeId = unsigned;

struct Endpoints {
  NodeId l, r;

  Endpoints();
  Endpoints(const NodeId&, const NodeId&);
  Endpoints(const Endpoints&);
  Endpoints& operator=(const Endpoints&);

  std::string dump() const;
};

struct EdgeInfo {
  std::string info;

  EdgeInfo();
  EdgeInfo(const std::string &);
  EdgeInfo(std::string &&);
  EdgeInfo(const EdgeInfo&);
  EdgeInfo(EdgeInfo&&);
  EdgeInfo& operator=(const EdgeInfo&);
  EdgeInfo& operator=(EdgeInfo&&);

  std::string dump() const;
};

bool operator==(const Endpoints &, const Endpoints &);
bool operator!=(const Endpoints &, const Endpoints &);

namespace std {
template <> struct hash<Endpoints> {
  size_t operator()(const Endpoints &e) const {
    return std::hash<NodeId>()(e.l) +
           (std::hash<NodeId>()(e.r) << 8 * sizeof(NodeId));
  }
};
}; // namespace std

/**
 * \brief \class AffineTx is an affine translation.
 */
struct AffineTx {
  C translate;
  double scale;

  AffineTx();
  void operator+=(C);      // effects translate
  void operator*=(double); // effects scale
};

struct BoundingRectangle {
  C upper_left;
  C lower_right;

  BoundingRectangle(C ul, C lr);

  double left() const;
  double right() const;
  double top() const;
  double bottom() const;
};

/**
 * \brief \class NodeInterface creates an interface with which nodes can be
 * positioned by drawing algorithms and drawn by the UI.
 */
class NodeInterface {
public:
  virtual NodeId get_Id() const = 0;

  virtual C get_position() const = 0;
  virtual void set_position(C) = 0;

  virtual Grid get_grid() const = 0;
  virtual void set_grid(Grid) = 0;

  virtual int get_alg_data() const = 0;
  virtual void set_alg_data(int) = 0;

  virtual std::string get_info() const = 0;

  virtual BoundingRectangle get_bounding_rectangle() = 0;
  virtual bool is_equal(const NodeInterface &) = 0;

  virtual ~NodeInterface() {}
};

bool intersects(const BoundingRectangle &r1, const BoundingRectangle &r2);
