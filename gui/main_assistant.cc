#include "main_assistant.h"

#include <gtkmm/main.h>

#include "assistant_file_page.h"

MainAssistant::MainAssistant ()
{
	set_default_size(600, 400);
	
	Gtk::Widget & first_page = *Gtk::manage (new AssistantFilePage ());
	append_page (first_page);
	set_page_type(first_page, Gtk::ASSISTANT_PAGE_INTRO);
	set_page_title(first_page, "Select impulse files");
	
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