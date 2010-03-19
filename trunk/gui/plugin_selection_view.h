#ifndef REVERB_TUBER_GUI_ASSISTANT_PLUGIN_PAGE_H
#define REVERB_TUBER_GUI_ASSISTANT_PLUGIN_PAGE_H

#include <gtkmm.h>

#include "reverbtuner/plugin.h"
#include "reverbtuner/lv2_plugin_factory.h"

class PluginSelectionView : public Gtk::VBox
{
  public:
	PluginSelectionView ();
	~PluginSelectionView ();
	
	ReverbTuner::PluginPtr get_plugin (double samplerate);
	
  private:
	void populate_plugin_list ();
	
	Gtk::ScrolledWindow plugin_scroller;
	Gtk::ListViewText plugin_list;
	
	ReverbTuner::Lv2PluginFactory factory;
	
};

#endif // REVERB_TUBER_GUI_ASSISTANT_PLUGIN_PAGE_H
