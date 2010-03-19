#include "reverbtuner/lv2_plugin_factory.h"

#include "reverbtuner/lv2_plugin.h"

namespace ReverbTuner {

Lv2PluginFactory::Lv2PluginFactory ()
{
}

unsigned
Lv2PluginFactory::plugin_count ()
{
	return slv2_plugins_size (world.plugins);
}

std::string
Lv2PluginFactory::plugin_name (unsigned index)
{
	SLV2Plugin plugin = slv2_plugins_get_at (world.plugins, index);
	return Lv2World::value_as_string (slv2_plugin_get_name (plugin));
}

boost::shared_ptr<Plugin>
Lv2PluginFactory::plugin (unsigned index, double samplerate)
{
	SLV2Plugin plugin = slv2_plugins_get_at (world.plugins, index);
	return boost::shared_ptr<Lv2Plugin> (new Lv2Plugin (world, plugin, samplerate));
}


} // namespace ReverbTuner
