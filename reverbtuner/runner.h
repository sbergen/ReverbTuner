#ifndef REVERB_TUNER_RUNNER_H
#define REVERB_TUNER_RUNNER_H

#include <boost/shared_ptr.hpp>

#include "reverbtuner/random_generator.h"

namespace ReverbTuner {

class DataSource;
class Plugin;
class PluginFactory;
class EvaluationScheduler;
class EvolutionaryOptimizer;
class EvaluationProgress;

class Runner
{
  public:
	Runner ();
	
	// These functions should be used in this order
	void set_data_source (boost::shared_ptr<DataSource> new_data_source) { data_source = new_data_source; }
	void set_plugin (boost::shared_ptr<Plugin> new_plugin) { plugin = new_plugin; }
	
	boost::shared_ptr<EvaluationProgress> start ();
	
  private:
	RandomGenerator rg;
	
	boost::shared_ptr<DataSource> data_source;
	boost::shared_ptr<Plugin> plugin;
	boost::shared_ptr<EvaluationScheduler> scheduler;
	boost::shared_ptr<EvolutionaryOptimizer> optimizer;
	boost::shared_ptr<EvaluationProgress> progress;

};

} // namespace ReverbTuner

#endif // REVERB_TUNER_RUNNER_H
