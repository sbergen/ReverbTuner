#ifndef REVERB_TUBER_GUI_PROGRESS_VIEW_H
#define REVERB_TUBER_GUI_PROGRESS_VIEW_H

#include <gtkmm.h>

#include "reverbtuner/evaluation_progress.h"

class ProgressView : public Gtk::VBox
{
  public:
	ProgressView ();
	~ProgressView ();
	
	typedef boost::shared_ptr<ReverbTuner::EvaluationProgress> ProgressPtr;
	void start (ProgressPtr progress_struct);
	
	sigc::signal<void> signal_aborted;
	
  private:
	
	void abort ();
	gint progress_timeout ();
	sigc::connection progress_connection;
	
	ProgressPtr progress;
	
	Gtk::ProgressBar bar;
	Gtk::Button abort_button;
	
};

#endif // REVERB_TUBER_GUI_PROGRESS_VIEW_H
