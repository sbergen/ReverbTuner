#include "reverbtuner/lv2_plugin_factory.h"
#include "reverbtuner/lv2_plugin.h"
#include "reverbtuner/data_source.h"
#include "reverbtuner/mfcc_evaluator.h"
#include "reverbtuner/single_threaded_scheduler.h"

#include <iostream>

#include <boost/shared_ptr.hpp>

using namespace ReverbTuner;

int main ()
{
	std::string wet, dry;
	std::cout << "Enter filename for wet file: ";
	std::cin >> wet;
	std::cout << "Enter filename for dry file: ";
	std::cin >> dry;
	
	if (dry.length() == 1) { dry = "samples/impulse.wav"; }
	if (wet.length() == 1) { wet = "samples/sample.wav"; }
	
	std::cout << "Loading files..." << std::endl;
	
	DataSource data_source (dry, wet);
	
	std::cout << "Loading plugins..." << std::endl;
	
	Lv2PluginFactory factory (data_source.get_samplerate ());
	
	unsigned count = factory.plugin_count ();
	
	std::cout << count << " Plugins found:" << std::endl;
	
	for (unsigned i = 0; i < count; ++i) {
		std::cout << i << ". " << factory.plugin_name (i) << std::endl;
	}
	
	unsigned which;
	do {
		std::cout << "Which plugin do you want to load? ";
		std::cin >> which;
	} while (which > count);
	
	boost::shared_ptr<Lv2Plugin> plugin = factory.plugin (which);
	data_source.set_plugin (plugin);
	
	std::cout << "Instantiating evaluator for plugin \"" << factory.plugin_name (which) << "\"" << std::endl;
	
	SingleThreadedScheduler<MfccEvaluator> scheduler (data_source);
	EvaluationSet set (plugin->get_parameters());
	set.resize (1);
	
	//ParameterValues values (plugin->get_parameters());
	//EvaluationResult result;
	
	std::cout << "Running evaluator.." << std::endl;
	
	scheduler.evaluate (set);
	
	//std::cout << "Got result: " << (float) result << std::endl;
	
	std::cout << "Done!" << std::endl;
}
