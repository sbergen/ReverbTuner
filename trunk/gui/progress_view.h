#ifndef REVERB_TUNER_GUI_PROGRESS_VIEW_H
#define REVERB_TUNER_GUI_PROGRESS_VIEW_H

#include <gtkmm.h>

#include "reverbtuner/evaluation_progress.h"
#include "reverbtuner/types.h"

#include "parameter_view.h"

class ProgressView : public Gtk::VBox
{
  public:
	ProgressView (ReverbTuner::Runner & runner);
	~ProgressView ();
	
	void start (ReverbTuner::SharedEvaluationProgressPtr progress_struct);
	
	sigc::signal<void> signal_aborted;
	sigc::signal<void, Gtk::Widget &, bool> complete_changed;
	
  private:
	
	void abort ();
	gint progress_timeout ();
	sigc::connection progress_connection;
	
	ReverbTuner::SharedEvaluationProgressPtr progress;
	
	Gtk::ProgressBar bar;
	Gtk::Button abort_button;
	
	ParameterView param_view;
	
};

#endif // REVERB_TUNER_GUI_PROGRESS_VIEW_H
