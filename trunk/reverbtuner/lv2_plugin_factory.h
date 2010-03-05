#ifndef REVERB_TUNER_LV2_PLUGIN_FACTORY_H
#define REVERB_TUNER_LV2_PLUGIN_FACTORY_H

#include <string>
#include <slv2.h>

namespace ReverbTuner {

class Lv2Plugin;

class Lv2PluginFactory
{
  public:

	Lv2PluginFactory (double samplerate);
	~Lv2PluginFactory ();
	
	unsigned plugin_count ();
	std::string plugin_name (unsigned index);
	
	/// Caller has to delete plugin
	Lv2Plugin * plugin (unsigned index);

  private:
	double samplerate;
	
	SLV2World  world;
	SLV2Plugin plugins;

};

} // namespace ReverbTuner

#endif // REVERB_TUNER_LV2_PLUGIN_FACTORY_H
