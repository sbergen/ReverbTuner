#include "reverbtuner/lv2_plugin_factory.h"

#include "reverbtuner/lv2_plugin.h"

namespace ReverbTuner {

Lv2PluginFactory::Lv2PluginFactory (double samplerate)
  : samplerate (samplerate)
{
	world = slv2_world_new ();
	slv2_world_load_all (world);
	plugins = slv2_world_get_all_plugins (world);
}

Lv2PluginFactory::~Lv2PluginFactory ()
{
	slv2_plugins_free (plugins);
	slv2_world_free (world);
}

unsigned
Lv2PluginFactory::plugin_count ()
{
	return slv2_plugins_size (plugins);
}

const char *
Lv2PluginFactory::plugin_name (unsigned index)
{
	SLV2Plugin plugin = slv2_plugins_get_at (index);
	SLV2Value value = slv2_plugin_get_name (plugin);
	std::string str(slv2_value_as_string (value));
	slv2_value_free (value);
	return str;
}

Lv2Plugin *
Lv2PluginFactory::plugin (unsigned index)
{
	SLV2Plugin plugin = slv2_plugins_get_at (index);
	return new Lv2Plugin (plugin, samplerate);
}


} // namespace ReverbTuner

#endif // REVERB_TUNER_LV2_PLUGIN_FACTORY_H
