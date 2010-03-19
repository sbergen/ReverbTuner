#include "plugin_selection_view.h"

PluginSelectionView::PluginSelectionView ()
  : plugin_list (1)
{
	populate_plugin_list ();
	
	plugin_scroller.set_policy (Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	plugin_scroller.add (plugin_list);
	pack_start (plugin_scroller, true, true);
}

PluginSelectionView::~PluginSelectionView ()
{

}

ReverbTuner::SharedPluginPtr
PluginSelectionView::get_plugin (double samplerate)
{
	Gtk::ListViewText::SelectionList selected = plugin_list.get_selected ();
	if (!selected.size ()) { return ReverbTuner::SharedPluginPtr (); }
	return factory.plugin (selected[0], samplerate);
}

void
PluginSelectionView::populate_plugin_list ()
{
	plugin_list.set_column_title (0, "Plugin name");
	unsigned count = factory.plugin_count ();
	for (unsigned i = 0; i < count; ++i) {
		plugin_list.append_text (factory.plugin_name (i));
	}
}
