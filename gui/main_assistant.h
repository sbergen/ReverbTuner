#ifndef REVERB_TUBER_GUI_MAIN_ASSISTANT_H
#define REVERB_TUBER_GUI_MAIN_ASSISTANT_H

#include <gtkmm/assistant.h>

class MainAssistant : public Gtk::Assistant
{
  public:
	MainAssistant ();
	~MainAssistant ();
	
  private:
	
	// Event handlers
	void on_cancel () { close (); }
	void on_close () { close (); }
	bool on_delete_event (GdkEventAny * event) { close (); return true; }
	
	void close ();
};

#endif // REVERB_TUBER_GUI_MAIN_ASSISTANT_H
