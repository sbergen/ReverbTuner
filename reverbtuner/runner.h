#ifndef REVERB_TUNER_RUNNER_H
#define REVERB_TUNER_RUNNER_H

#include <string>
#include <boost/shared_ptr.hpp>

#include "reverbtuner/random_generator.h"

namespace ReverbTuner {

class DataSource;
class Plugin;
class PluginFactory;
class EvaluationScheduler;
class EvolutionaryOptimizer;

/** An interface for running the whole process
  * In addition to \a Plugin and \a PluginFactory,
  * this is the only class the UI needs to know about.
  */
class Runner
{
  public:
	Runner ();
	
	enum PluginType {
		PluginTypeLV2
	};
	
	enum SchedulerType {
		SchedulerSingleThreaded,
		SchedulerThreaded
	};
	
	enum EvaluatorType {
		EvaluatorMfcc
	};
	
	
	// These functions should be used in this order
	void set_files (std::string const & dry_filename, std::string const & wet_filename);
	boost::shared_ptr<PluginFactory> get_plugin_factory (PluginType type);
	void start (SchedulerType sched_type, EvaluatorType eval_type, unsigned concurrency);
	void stop ();
	
	// 
	
  private:
	RandomGenerator rg;
	
	boost::shared_ptr<DataSource> data_source;
	boost::shared_ptr<PluginFactory> plugin_factory;
	boost::shared_ptr<EvaluationScheduler> scheduler;
	boost::shared_ptr<EvolutionaryOptimizer> evolutionary_optimizer;

};

} // namespace ReverbTuner

#endif // REVERB_TUNER_RUNNER_H
