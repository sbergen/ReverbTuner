#ifndef REVERB_TUBER_GUI_MAIN_ASSISTANT_H
#define REVERB_TUBER_GUI_MAIN_ASSISTANT_H

#include <gtkmm/assistant.h>

#include "reverbtuner/runner.h"

class FileSelectionView;
class PluginSelectionView;
class ProgressView;

class MainAssistant : public Gtk::Assistant
{
  public:
	MainAssistant ();
	~MainAssistant ();
	
  private:
	
	void start_evaluation ();
	
	FileSelectionView & file_page;
	PluginSelectionView & plugin_page;
	ProgressView & progress_page;
	
	int progress_page_index;
	bool evaluation_started;
	
	ReverbTuner::Runner runner;
	
	// Callbacks
	void evaluation_aborted ();

  private:
	// Event handlers
	void on_cancel () { close (); }
	void on_close () { close (); }
	bool on_delete_event (GdkEventAny * event) { close (); return true; }
	void on_prepare (Gtk::Widget* page);
	
	void close ();
};

#endif // REVERB_TUBER_GUI_MAIN_ASSISTANT_H
