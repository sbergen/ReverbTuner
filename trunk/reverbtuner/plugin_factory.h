#ifndef REVERB_TUNER_PLUGIN_FACTORY
#define REVERB_TUNER_PLUGIN_FACTORY

#include <string>
#include <boost/shared_ptr.hpp>

namespace ReverbTuner {

class Plugin;

class PluginFactory
{
  public:
	virtual ~PluginFactory () {}
	
	virtual unsigned plugin_count () = 0;
	virtual std::string plugin_name (unsigned index) = 0;
	virtual boost::shared_ptr<Plugin> plugin (unsigned index) = 0;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_PLUGIN_FACTORY
