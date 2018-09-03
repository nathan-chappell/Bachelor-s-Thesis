// UI.cc

#include "UI.h"

#include <cmath>
#include <iostream>

using namespace std;

const int L_ARROW = 65361;
const int U_ARROW = 65362;
const int R_ARROW = 65363;
const int D_ARROW = 65364;
const int KEY_PLUS = 65451;
const int KEY_MINUS = 65453;

const double zoom_factor = 1.1;
const double PI = 4 * atan(1);

AffineTx::AffineTx() : translate(0.0, 0.0), scale(1.0) {}

void AffineTx::operator+=(C c) { translate += c; }
void AffineTx::operator*=(double d) { scale *= d; }

MyWindow::MyWindow(AffineTx &tx) : tx_(tx) {
  set_default_size(800, 800);
  add_events(Gdk::KEY_PRESS_MASK | Gdk::SCROLL_MASK);
}

bool MyWindow::on_key_press_event(GdkEventKey *e) {
  switch (e->keyval) {
  case GDK_KEY_Escape:
    close();
    break;
  case L_ARROW: // L
    tx_ += C(-10, 0);
    break;
  case U_ARROW: // U
    tx_ += C(0, 10);
    break;
  case R_ARROW: // R
    tx_ += C(10, 0);
    break;
  case D_ARROW: // D
    tx_ += C(0, -10);
    break;
  case KEY_PLUS:
    tx_.scale *= zoom_factor;
    break;
  case KEY_MINUS:
    tx_.scale /= zoom_factor;
    break;
  default: { cout << "button pressed: " << e->keyval << endl; }
  }
  // cout << "tx_: " << tx_ << ", tx_.scale:" << tx_.scale << endl;
  queue_draw();
  return false;
}

bool MyWindow::on_scroll_event(GdkEventScroll *s) {
  queue_draw();
  return false;
}

MyDrawingArea::MyDrawingArea(AffineTx &tx) : tx_(tx), drag_(false) {
  click_screen = [](double, double) { return true; }; // dummy
  add_events(Gdk::SCROLL_MASK | Gdk::BUTTON_PRESS_MASK |
             Gdk::BUTTON_RELEASE_MASK | Gdk::POINTER_MOTION_MASK);
}

bool MyDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context> &c) {
  c->translate(tx_.translate.real(), tx_.translate.imag());
  c->scale(tx_.scale, tx_.scale);
  c->arc(0, 0, 30, 0, 2 * PI);
  c->stroke();
  return false;
}

bool MyDrawingArea::on_motion_notify_event(GdkEventMotion *e) {
  if (drag_) {
    C this_click(e->x, e->y);
    tx_ += (this_click - last_pointer_);
    last_pointer_ = this_click;
    queue_draw();
  }
  return false;
}

bool MyDrawingArea::on_button_press_event(GdkEventButton *e) {
  if (click_screen(e->x, e->y)) {
    drag_ = true;
    last_pointer_ = C(e->x, e->y);
  }
  return false;
}

bool MyDrawingArea::on_button_release_event(GdkEventButton *e) {
  drag_ = false;
  return false;
}

void zoom_at(AffineTx &tx, double x, double y, bool up) {
  double factor;
  if (up)
    factor = zoom_factor;
  else
    factor = 1 / zoom_factor;

  double new_x = x - factor * (x - tx.translate.real());
  double new_y = y - factor * (y - tx.translate.imag());
  tx.translate = C(new_x, new_y);
  tx.scale *= factor;
}

bool MyDrawingArea::on_scroll_event(GdkEventScroll *e) {
  zoom_at(tx_, e->x, e->y, e->direction == GDK_SCROLL_UP);
  return false;
}

UiController::UiController() : window(tx_), drawingArea(tx_) {
  window.add(drawingArea);
  window.show_all();
}

const MyDrawingArea &UiController::DrawingArea() const { return drawingArea; }

MyWindow &UiController::Window() { return window; }

const AffineTx &UiController::Tx() const { return tx_; }

void UiController::Tx(AffineTx &tx_) { tx_ = tx_; }
