#include "main_assistant.h"

#include <iostream>

#include <gtkmm/main.h>

#include "reverbtuner/runner.h"

#include "file_selection_view.h"
#include "plugin_selection_view.h"
#include "progress_view.h"

MainAssistant::MainAssistant ()
  : file_page (*Gtk::manage (new FileSelectionView (runner)))
  , plugin_page (*Gtk::manage (new PluginSelectionView ()))
  , progress_page (*Gtk::manage (new ProgressView ()))
  , progress_page_index (-1)
  , evaluation_started (false)
{
	set_default_size(600, 400);
	
	append_page (file_page);
	set_page_title(file_page, "Select impulse files");
	file_page.complete_changed.connect (mem_fun (*this, &MainAssistant::set_page_complete));
	
	append_page (plugin_page);
	set_page_title(plugin_page, "Select plugin");
	set_page_complete (plugin_page, true);
	
	progress_page_index = get_n_pages ();
	append_page (progress_page);
	set_page_title (progress_page, "Evaluation running...");
	set_page_type (progress_page, Gtk::ASSISTANT_PAGE_PROGRESS);
	progress_page.signal_aborted.connect (mem_fun (*this, &MainAssistant::evaluation_aborted));
	progress_page.complete_changed.connect (mem_fun (*this, &MainAssistant::set_page_complete));
	
	show_all();
}

MainAssistant::~MainAssistant ()
{

}

void
MainAssistant::start_evaluation ()
{
	if (evaluation_started) { return; }
	
	ReverbTuner::PluginPtr plugin = plugin_page.get_plugin (file_page.file_samplerate ());
	runner.set_plugin (plugin);
	
	progress_page.start (runner.start ());
	
	evaluation_started = true;
}

void
MainAssistant::evaluation_aborted ()
{
	evaluation_started = false;
}

void
MainAssistant::on_prepare (Gtk::Widget* page)
{
	if (page && (page == &progress_page)) {
		start_evaluation ();
	}
}

void
MainAssistant::close ()
{
	hide ();
	Gtk::Main::quit ();
}
