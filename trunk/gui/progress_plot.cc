#include "progress_plot.h"

#include <cairomm/context.h>

#include <cmath>

ProgressPlot::ProgressPlot(unsigned width_in_points)
  : length (width_in_points)
{
}

ProgressPlot::~ProgressPlot()
{
}

void
ProgressPlot::add_progress_point (double value)
{
	if (data.size () == 0) { scale_min = std::max (50.0, value); }
	if (data.size () >= length) { data.pop_front (); }
	data.push_back (value);
	redraw ();
}

double
ProgressPlot::scale_x (double x, double total_width)
{
	return x * total_width / length;
}

double
ProgressPlot::scale_y (double y, double total_height)
{
	y = -y + 100.0; // flip and re-bias to be positive
	return std::min (y / (100 - scale_min) * total_height, total_height);
}

void
ProgressPlot::redraw ()
{
	Glib::RefPtr<Gdk::Window> window = get_window ();
	if (window) {
		Gdk::Rectangle r (0, 0, get_allocation().get_width(),
		                  get_allocation().get_height());
        window->invalidate_rect (r, false);
	}
}

bool
ProgressPlot::on_expose_event(GdkEventExpose* event)
{
	if (!data.size()) { return true; }
	
	Glib::RefPtr<Gdk::Window> window = get_window ();
	if (!window) { return true; }

	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();

	Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
	cr->set_line_width (0.5);

	// clip to the area indicated by the expose event so that we only redraw
	// the portion of the window that needs to be redrawn
	cr->rectangle(event->area.x, event->area.y,
			event->area.width, event->area.height);
	cr->clip();
	
	// Draw
	cr->set_source_rgb (0.0, 0.0, 0.8);
	cr->move_to (0, scale_y (data.front(), height));
	unsigned pos = 0;
	for (std::list<float>::const_iterator it = data.begin(); it != data.end(); ++it, ++pos) {
		cr->line_to (scale_x (pos, width), scale_y (*it, height));
	}
	
	cr->stroke ();
	return true;
}
