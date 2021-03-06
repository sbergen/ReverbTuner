#include "reverbtuner/lv2_plugin_factory.h"
#include "reverbtuner/plugin.h"
#include "reverbtuner/data_source.h"
#include "reverbtuner/mfcc_evaluator.h"
#include "reverbtuner/threaded_scheduler.h"
#include "reverbtuner/single_threaded_scheduler.h"
#include "reverbtuner/evolutionary_optimizer.h"
#include "reverbtuner/random_generator.h"
#include "reverbtuner/evaluation_progress.h"

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
	
	boost::shared_ptr<Plugin> plugin = factory.plugin (which);
	data_source.set_plugin (plugin);
	
	std::cout << "Instantiating evaluator for plugin \"" << factory.plugin_name (which) << "\"" << std::endl;
	
	//SingleThreadedScheduler<MfccEvaluator> scheduler (data_source);
	ThreadedScheduler<MfccEvaluator> scheduler (data_source);
	scheduler.alloc_resources ();
	RandomGenerator rg;
	EvolutionaryOptimizer optimizer (data_source, scheduler, rg);
	
	std::cout << "Running evaluator.." << std::endl;
	
	boost::shared_ptr<EvaluationProgress> progress (new EvaluationProgress ());
	optimizer.run (progress);
	
	std::cout << "Done!" << std::endl;
}
