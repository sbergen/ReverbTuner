#ifndef REVERB_TUNER_LV2_PLUGIN_FACTORY_H
#define REVERB_TUNER_LV2_PLUGIN_FACTORY_H

#include <slv2/slv2.h>

#include "plugin_factory.h"
#include "lv2_world.h"

namespace ReverbTuner {

class Lv2PluginFactory : public PluginFactory
{
  public:

	Lv2PluginFactory ();
	
	unsigned plugin_count ();
	std::string plugin_name (unsigned index);
	boost::shared_ptr<Plugin> plugin (unsigned index, double samplerate);

  private:
	Lv2World    world;

};

} // namespace ReverbTuner

#endif // REVERB_TUNER_LV2_PLUGIN_FACTORY_H
