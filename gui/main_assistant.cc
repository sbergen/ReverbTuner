#include "main_assistant.h"

#include <iostream>

#include <gtkmm/main.h>

#include "assistant_file_page.h"
#include "assistant_plugin_page.h"

MainAssistant::MainAssistant ()
{
	set_default_size(600, 400);
	
	AssistantFilePage & file_page = *Gtk::manage (new AssistantFilePage ());
	append_page (file_page);
	set_page_title(file_page, "Select impulse files");
	file_page.complete_changed.connect (mem_fun (*this, &MainAssistant::set_page_complete));
	
	Gtk::Widget & plugin_page = *Gtk::manage (new AssistantPluginPage ());
	append_page (plugin_page);
	set_page_title(plugin_page, "Select plugin");
	
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