// UI.h
#pragma once

#include <gtkmm-3.0/gtkmm.h>

#include "common_graph.h"

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

/**
 * \brief \class MyDrawingArea is derived from the Gtk and offers additional
 * functionality.
 */
class MyDrawingArea : public Gtk::DrawingArea {
private:
  AffineTx &tx_;
  bool drag_;
  C last_pointer_;

public:
  /**
   * \brief \function callback to see if a user click has clicked any object, or
   * just the screen
   */
  bool (*click_screen)(double x, double y);

  MyDrawingArea(AffineTx &tx);

protected:
  bool on_draw(const Cairo::RefPtr<Cairo::Context> &c) override;
  bool on_button_press_event(GdkEventButton *e) override;
  bool on_button_release_event(GdkEventButton *e) override;
  bool on_motion_notify_event(GdkEventMotion *e) override;
  bool on_scroll_event(GdkEventScroll *e) override;
};

/**
 * \brief \class MyWindow is derived from the Gtk and offers additional
 * functionality.
 */
class MyWindow : public Gtk::Window {
private:
  AffineTx &tx_;

public:
  MyWindow(AffineTx &tx);

  bool on_key_press_event(GdkEventKey *e) override;
  bool on_scroll_event(GdkEventScroll *s) override;
};

/** \class UiController MyDrawingArea and MyWindow must share an AffineTx, the
 * UiController manages this mutual dependency.
 */
class UiController {
private:
  AffineTx tx_;
  MyWindow window;
  MyDrawingArea drawingArea;

public:
  UiController();

  // get
  const MyDrawingArea &DrawingArea() const;
  /* not const to allow for Gtk::Application to run the window */
  MyWindow &Window();
  const AffineTx &Tx() const;

  // set
  void Tx(AffineTx &);
};
