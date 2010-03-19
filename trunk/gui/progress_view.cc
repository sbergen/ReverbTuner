#include "progress_view.h"


ProgressView::ProgressView ()
{
	pack_start (bar, false, false);
	pack_start (abort_button, false, false);
	
	abort_button.signal_clicked ().connect (sigc::mem_fun (*this, &ProgressView::abort));
}

ProgressView::~ProgressView ()
{
	progress_connection.disconnect ();
}

void
ProgressView::start (ProgressPtr progress_struct)
{
	progress = progress_struct;
	progress_connection = Glib::signal_timeout().connect (sigc::mem_fun (*this, &ProgressView::progress_timeout), 100);
}

void
ProgressView::abort ()
{
	if (progress) {
		progress->abort ();
	}
	
	progress_connection.disconnect ();
	signal_aborted ();
}

gint
ProgressView::progress_timeout ()
{
	unsigned total = progress->total_rounds ();
	unsigned current = progress->current_round ();
	
	bar.set_fraction (static_cast<double> (current) / total);
	return TRUE;
}
