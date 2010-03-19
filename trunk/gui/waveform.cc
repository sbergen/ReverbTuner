#include "waveform.h"

#include <cairomm/context.h>

Waveform::Waveform()
  : data (0)
  , length (0)
{
}

Waveform::~Waveform()
{
}

void
Waveform::set_data (std::vector<float> const & new_data, unsigned new_length)
{
	data = &new_data;
	length = new_length;
	redraw ();
}

double
Waveform::scale_x (double x, double total_width)
{
	if (!data) { return 0; }
	double total_length = length > 0 ? length : data->size ();
	return total_width * x / total_length;
}

double
Waveform::scale_y (double y, double total_height)
{
	// data scale [-1, 1]
	y = -y;   // flip around
	y += 1.0; // bias to [0, 2]
	return y / 2.0 * total_height;
}

void
Waveform::redraw ()
{
	Glib::RefPtr<Gdk::Window> window = get_window ();
	if (window) {
		Gdk::Rectangle r (0, 0, get_allocation().get_width(),
		                  get_allocation().get_height());
        window->invalidate_rect (r, false);
	}
}

bool
Waveform::on_expose_event(GdkEventExpose* event)
{
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
	cr->move_to (0, scale_y (0.0, height));
	
	// Straight line if no data
	if (!data) {
		cr->line_to (width, scale_y (0.0, height));
		cr->stroke ();
		return true;
	}
	
	// First data
	unsigned pos;
	for (pos = 0; pos < data->size(); ++pos) {
		cr->line_to (scale_x (pos, width), scale_y ((*data)[pos], height));
	}
	
	// Then zeros
	cr->line_to (++pos, scale_y (0.0, height));
	cr->line_to (width, scale_y (0.0, height));
	
	cr->stroke ();
	return true;
}
