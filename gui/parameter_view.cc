#include "parameter_view.h"

#include "reverbtuner/parameter_values.h"
#include "reverbtuner/parameter_set.h"

using namespace ReverbTuner;

ParameterView::ParameterView (ReverbTuner::Runner & runner)
  : runner (runner)
  , model_ready (false)
{
	model = Gtk::ListStore::create (param_cols);
	view.set_model (model);
	view.append_column ("Name", param_cols.name);
	view.append_column ("Value", param_cols.value);
	
	pack_start (view);
}

ParameterView::~ParameterView ()
{

}

void
ParameterView::update ()
{
	runner.get_best_params (values);
	if (!values) { return; }
	if (!model_ready) { init_model (); }
	
	for (Gtk::TreeModel::iterator it = model->children ().begin (); it != model->children ().end (); ++it) {
		(*it)[param_cols.value] = (*values)[(*it)[param_cols.index]];
	}
}

void
ParameterView::init_model ()
{
	if (!values) { return; }
	
	model->clear ();
	ParameterSet const & set = values->get_set ();
	for (ParameterValues::const_iterator it = values->begin (); it != values->end (); ++it) {
		unsigned idx = it->first;
		Gtk::TreeModel::iterator iter = model->append ();
		Gtk::TreeModel::Row row = *iter;
		row[param_cols.index] = idx;
		row[param_cols.name] = set[idx].name ();
		row[param_cols.value] = (*values)[idx];
	}
	
	model_ready = true;
}
