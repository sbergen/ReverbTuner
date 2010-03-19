#ifndef REVERB_TUNER_GUI_PARAMETER_VIEW_H
#define REVERB_TUNER_GUI_PARAMETER_VIEW_H

#include <gtkmm.h>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

#include "reverbtuner/runner.h"
#include "reverbtuner/types.h"

class ParameterView : public Gtk::VBox
{
  public:
	ParameterView (ReverbTuner::Runner & runner);
	~ParameterView ();
	
	void update ();
	void reset ();
	
  private:
	
	void init_model ();
	
	ReverbTuner::ScopedParameterValuesPtr values;
	ReverbTuner::Runner & runner;
	
	// GUI
	
	struct ParameterColumns : public Gtk::TreeModel::ColumnRecord
	{
	public:
		Gtk::TreeModelColumn<unsigned>      index;
		Gtk::TreeModelColumn<Glib::ustring> name;
		Gtk::TreeModelColumn<float>         value;

		ParameterColumns() { add (index); add (name); add (value); }
	};
	ParameterColumns param_cols;
	Gtk::TreeView view;
	Glib::RefPtr<Gtk::ListStore> model;
	bool model_ready;
};

#endif // REVERB_TUNER_GUI_PARAMETER_VIEW_H
