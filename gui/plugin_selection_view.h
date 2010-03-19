#ifndef REVERB_TUNER_GUI_PLUGIN_SELECTION_VIEW_H
#define REVERB_TUNER_GUI_PLUGIN_SELECTION_VIEW_H

#include <gtkmm.h>

#include "reverbtuner/types.h"
#include "reverbtuner/lv2_plugin_factory.h"

class PluginSelectionView : public Gtk::VBox
{
  public:
	PluginSelectionView ();
	~PluginSelectionView ();
	
	ReverbTuner::SharedPluginPtr get_plugin (double samplerate);
	
  private:
	void populate_plugin_list ();
	
	Gtk::ScrolledWindow plugin_scroller;
	Gtk::ListViewText plugin_list;
	
	ReverbTuner::Lv2PluginFactory factory;
	
};

#endif // REVERB_TUNER_GUI_PLUGIN_SELECTION_VIEW_H
