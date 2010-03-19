#ifndef REVERB_TUBER_GUI_WAVEFORM_H
#define REVERB_TUBER_GUI_WAVEFORM_H

#include <vector>

#include <gtkmm/drawingarea.h>

class Waveform : public Gtk::DrawingArea
{
  public:
	Waveform ();
	~Waveform ();

	void reset_data () { data = 0; length = 0; redraw (); }
	void set_data (std::vector<float> const & new_data, unsigned new_length = 0);
	
  private:
	
	double scale_x (double x, double total_width);
	double scale_y (double y, double total_height);
	
	void redraw ();
	bool on_expose_event (GdkEventExpose* event);
	
	std::vector<float> const * data;
	unsigned length;
};

#endif // GTKMM_EXAMPLE_MYAREA_H
