#include <gtkmm/main.h>

#include "main_assistant.h"

int
main(int argc, char ** argv) {
	Gtk::Main kit(argc, argv);
	MainAssistant assistant;
	Gtk::Main::run (assistant);
}