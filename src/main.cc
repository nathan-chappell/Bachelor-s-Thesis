//main.cc

#include "UI.h"
#include "drawing_algorithms.h"

#include <gtkmm-3.0/gtkmm.h>

int main(int argc, char **argv) {

  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create();

  UiController controller;

  app->run(controller.Window());
}
