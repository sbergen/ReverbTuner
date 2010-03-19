#ifndef REVERB_TUNER_GUI_PROGRESS_PLOT_H
#define REVERB_TUNER_GUI_PROGRESS_PLOT_H

#include <list>

#include <gtkmm/drawingarea.h>

class ProgressPlot : public Gtk::DrawingArea
{
  public:
	ProgressPlot (unsigned width_in_points = 100);
	~ProgressPlot ();

	void add_progress_point (double value);
	void reset () { data.clear (); }
	
  private:
	
	double scale_x (double x, double total_width);
	double scale_y (double y, double total_height);
	
	void redraw ();
	bool on_expose_event (GdkEventExpose* event);
	
	std::list<float> data;
	unsigned length;
	double scale_min;
};

#endif // REVERB_TUNER_GUI_PROGRESS_PLOT_H
