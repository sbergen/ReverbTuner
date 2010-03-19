#include "progress_view.h"


ProgressView::ProgressView (ReverbTuner::Runner & runner)
  : abort_button ("Stop evaluation")
  , param_view (runner)
{
	pack_start (bar, false, false);
	pack_start (abort_button, false, false);
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
	param_view.update ();
	
	unsigned total = progress->total_rounds ();
	unsigned current = progress->current_round ();
	
	bar.set_fraction (static_cast<double> (current) / total);
	
	if (progress->done ()) {
		complete_changed (*this, true);
		return FALSE;
	}
	
	return TRUE;
}
