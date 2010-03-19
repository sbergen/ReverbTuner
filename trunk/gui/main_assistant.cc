#include "main_assistant.h"

#include <iostream>

#include <gtkmm/main.h>

#include "file_selection_view.h"
#include "plugin_selection_view.h"

MainAssistant::MainAssistant ()
{
	set_default_size(600, 400);
	
	FileSelectionView & file_page = *Gtk::manage (new FileSelectionView ());
	append_page (file_page);
	set_page_title(file_page, "Select impulse files");
	file_page.complete_changed.connect (mem_fun (*this, &MainAssistant::set_page_complete));
	
	PluginSelectionView & plugin_page = *Gtk::manage (new PluginSelectionView ());
	append_page (plugin_page);
	set_page_title(plugin_page, "Select plugin");
	set_page_complete (plugin_page, true);
	
	show_all();
}

MainAssistant::~MainAssistant ()
{

}

void
MainAssistant::close ()
{
	hide ();
	Gtk::Main::quit ();
}
