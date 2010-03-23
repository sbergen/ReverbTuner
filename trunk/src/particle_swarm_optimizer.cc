#include "reverbtuner/particle_swarm_optimizer.h"

#include "reverbtuner/data_source.h"
#include "reverbtuner/evaluation_progress.h"
#include "reverbtuner/evaluation_scheduler.h"
#include "reverbtuner/parameter_values.h"
#include "reverbtuner/plugin.h"

namespace ReverbTuner {

ParticleSwarmOptimizer::ParticleSwarmOptimizer (DataSource const & data_source, EvaluationScheduler & scheduler, RandomGenerator & rg)
  : evaluation_set (data_source.get_plugin()->get_parameters())
  , param_modifier (data_source.get_samplerate(), rg)
  , data_source (data_source)
  , scheduler (scheduler)
  , rg (rg)
  , rounds (100)
  , population_size (300)
  , velocity (0.005)
{

}

ParticleSwarmOptimizer::~ParticleSwarmOptimizer ()
{

}

void
ParticleSwarmOptimizer::run (SharedEvaluationProgressPtr progress_)
{
	progress = progress_;
	progress->set_total_rounds (rounds);
	boost::thread (boost::bind (&ParticleSwarmOptimizer::do_run, this));
}

bool
ParticleSwarmOptimizer::get_best_params (ScopedParameterValuesPtr & params)
{
	LockGuard lg (global_best_mutex);
	if (!global_best_values) { return false; }
	params.reset (new ParameterValues (*global_best_values));
	return true;
}

void
ParticleSwarmOptimizer::do_run ()
{
	ensure_population_size ();
	initialize_global_best ();
	
	for (unsigned i = 0; i < rounds; ++i) {
		if (progress->aborted()) { return; }
		
		progress->set_current_round (i + 1);
		scheduler.evaluate (evaluation_set);
		ensure_population_size ();
		move_particles ();
	}
	
	progress->set_done ();
}

void
ParticleSwarmOptimizer::ensure_population_size ()
{
	evaluation_set.resize (population_size,
		boost::bind (&ParameterModifier::randomize_all, param_modifier, _1, 1.0));
}

void
ParticleSwarmOptimizer::initialize_global_best ()
{
	for (SwarmEvaluationSet::iterator it = evaluation_set.begin (); it != evaluation_set.end (); ++it) {
		update_global_best (*it);
	}
}

void
ParticleSwarmOptimizer::move_particles ()
{
	for (SwarmEvaluationSet::iterator it = evaluation_set.begin (); it != evaluation_set.end (); ++it) {
		move_particle (*it);
		update_local_best (*it);
		update_global_best (*it);
	}
}

void
ParticleSwarmOptimizer::move_particle (SwarmEvaluationSet::Particle & particle)
{
	
}

void
ParticleSwarmOptimizer::update_local_best (SwarmEvaluationSet::Particle & particle)
{
	if (particle.result > particle.local_best_result) {
		particle.local_best_result = particle.result;
		particle.local_best_values = particle.values;
	}
}

void
ParticleSwarmOptimizer::update_global_best (SwarmEvaluationSet::Particle const & particle)
{
	if (particle.result > global_best_result) {
		LockGuard lg (global_best_mutex);
		global_best_result = particle.result;
		*global_best_values = particle.values;
	}
}

} // namespace ReverbTuner
