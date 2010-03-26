#include "reverbtuner/runner.h"

#include "reverbtuner/data_source.h"
#include "reverbtuner/evaluation_progress.h"
#include "reverbtuner/evolutionary_optimizer.h"
#include "reverbtuner/particle_swarm_optimizer.h"
#include "reverbtuner/mfcc_evaluator.h"
#include "reverbtuner/threaded_scheduler.h"

#include <stdexcept>

namespace ReverbTuner {

Runner::Runner ()
{

}

boost::shared_ptr<EvaluationProgress>
Runner::start ()
{
	if (!data_source || !plugin) { throw std::logic_error ("Runner not preapred"); }
	
	data_source->set_plugin (plugin);
	
	progress.reset (new EvaluationProgress ());
	scheduler.reset (new ThreadedScheduler<MfccEvaluator> (*data_source));
	//optimizer.reset (new EvolutionaryOptimizer (*data_source, *scheduler, rg));
	optimizer.reset (new ParticleSwarmOptimizer (*data_source, *scheduler, rg));
	
	scheduler->alloc_resources (boost::thread::hardware_concurrency());
	optimizer->run (progress);
	
	return progress;
}

bool
Runner::get_best_params (ScopedParameterValuesPtr & params)
{
	if (!optimizer) { return false; }
	return optimizer->get_best_params (params);
}

} // namespace ReverbTuner

