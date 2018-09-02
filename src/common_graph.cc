// common.cc

#include "common_graph.h"

using namespace std;

Endpoints::Endpoints()
    : l(numeric_limits<unsigned>::max()), r(numeric_limits<unsigned>::max()) {}

Endpoints::Endpoints(const NodeId& l, const NodeId& r) : l(l), r(r) {}

Endpoints::Endpoints(const Endpoints& e) {
  *this = e;
}

Endpoints& Endpoints::operator=(const Endpoints& e) {
  l = e.l;
  r = e.r;
  return *this;
}

string Endpoints::dump() const {
  return "[" + to_string(l) + ", " + to_string(r) + "]";
}

EdgeInfo::EdgeInfo() {}
EdgeInfo::EdgeInfo(const string &info) : info(info) {}
EdgeInfo::EdgeInfo(string &&info) : info(move(info)) {}
EdgeInfo::EdgeInfo(const EdgeInfo &ed) : info(ed.info) {}
EdgeInfo::EdgeInfo(EdgeInfo &&ed) : info(move(ed.info)) {}

EdgeInfo& EdgeInfo::operator=(const EdgeInfo& e) {
  info = e.info;
  return *this;
}

EdgeInfo& EdgeInfo::operator=(EdgeInfo&& e) {
  info = move(e.info);
  return *this;
}

string EdgeInfo::dump() const { return "\"" + info + "\""; }

bool operator==(const Endpoints &e1, const Endpoints &e2) {
  return (e1.l == e2.l) && (e1.r == e2.r);
}

bool operator!=(const Endpoints &e1, const Endpoints &e2) {
  return !(e1 == e2);
}

AffineTx::AffineTx() : translate(0.0, 0.0), scale(1.0) {}

void AffineTx::operator+=(C c) { translate += c; }
void AffineTx::operator*=(double d) { scale *= d; }

BoundingRectangle::BoundingRectangle(C ul, C lr)
    : upper_left(ul), lower_right(lr) {}

double BoundingRectangle::left() const { return upper_left.real(); }
double BoundingRectangle::right() const { return lower_right.real(); }
double BoundingRectangle::top() const { return upper_left.imag(); }
double BoundingRectangle::bottom() const { return lower_right.imag(); }

bool is_above(const BoundingRectangle &r1, const BoundingRectangle &r2) {
  return r1.bottom() < r2.top(); // gtkmm inverts y axis!
}

bool is_below(const BoundingRectangle &r1, const BoundingRectangle &r2) {
  return r1.top() > r2.bottom(); // gtkmm inverts y axis!
}

bool is_right_of(const BoundingRectangle &r1, const BoundingRectangle &r2) {
  return r1.left() > r2.right();
}

bool is_left_of(const BoundingRectangle &r1, const BoundingRectangle &r2) {
  return r1.right() < r2.left();
}

bool intersects(const BoundingRectangle &r1, const BoundingRectangle &r2) {
  return !(is_above(r1, r2) || is_below(r1, r2) || is_right_of(r1, r2) ||
           is_left_of(r1, r2));
}
