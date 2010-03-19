#include "progress_view.h"


ProgressView::ProgressView (ReverbTuner::Runner & runner)
  : abort_button ("Stop evaluation")
  , param_view (runner)
  , last_drawn_round (1)
{
	pack_start (bar, false, false);
	pack_start (abort_button, false, false);
	pack_start (progress_plot, true, true);
	pack_start (param_view, false, false);
	
	abort_button.set_sensitive (false);
	abort_button.signal_clicked ().connect (sigc::mem_fun (*this, &ProgressView::abort));
}

ProgressView::~ProgressView ()
{
	progress_connection.disconnect ();
}

void
ProgressView::start (ReverbTuner::SharedEvaluationProgressPtr progress_struct)
{
	last_drawn_round = 1;
	progress_plot.reset ();
	param_view.reset ();
	progress = progress_struct;
	progress_connection = Glib::signal_timeout().connect (sigc::mem_fun (*this, &ProgressView::progress_timeout), 100);
	abort_button.set_sensitive (true);
	complete_changed (*this, false);
}

void
ProgressView::abort ()
{
	if (progress) {
		progress->abort ();
	}
	
	progress_connection.disconnect ();
	signal_aborted ();
	abort_button.set_sensitive (false);
	complete_changed (*this, true);
}

gint
ProgressView::progress_timeout ()
{
	// Check whether we need to do anything
	if (!progress) { return TRUE; }
	unsigned current = progress->current_round ();
	if (current <= last_drawn_round) { return TRUE; }
	last_drawn_round = current;
	
	// Update sub views
	progress_plot.add_progress_point (progress->best_result ());
	param_view.update ();
	
	// Update bar
	unsigned total = progress->total_rounds ();
	bar.set_fraction (static_cast<double> (current) / total);
	
	// stop if ready
	if (progress->done ()) {
		progress_connection.disconnect ();
		abort_button.set_sensitive (false);
		complete_changed (*this, true);
		return FALSE;
	}
	
	return TRUE;
}
