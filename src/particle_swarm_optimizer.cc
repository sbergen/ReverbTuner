#include "reverbtuner/particle_swarm_optimizer.h"

#include "reverbtuner/data_source.h"
#include "reverbtuner/evaluation_progress.h"
#include "reverbtuner/evaluation_scheduler.h"
#include "reverbtuner/parameter.h"
#include "reverbtuner/parameter_set.h"
#include "reverbtuner/parameter_values.h"
#include "reverbtuner/plugin.h"
#include "reverbtuner/random_generator.h"

namespace ReverbTuner {

ParticleSwarmOptimizer::ParticleSwarmOptimizer (DataSource const & data_source, EvaluationScheduler & scheduler, RandomGenerator & rg)
  : evaluation_set (data_source.get_plugin()->get_parameters())
  , param_modifier (data_source.get_samplerate(), rg)
  , data_source (data_source)
  , scheduler (scheduler)
  , rg (rg)
  , rounds (100)
  , population_size (300)
  , initial_velocity (0.01)
  , velocity_change_rate (0.1)
  , global_velocity_weight (0.3)
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
		boost::bind (&ParameterModifier::randomize_all, param_modifier, _1, 1.0),
		boost::bind (&ParticleSwarmOptimizer::init_random_velocity_for_set, this, _1, _2));
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
ParticleSwarmOptimizer::move_particle (Particle & particle)
{
	update_particle_velocity (particle);
	
	// TODO edges!
	
	particle.values += particle.velocity;
	
}

void
ParticleSwarmOptimizer::update_particle_velocity (Particle & particle)
{
	ParticleVelocity local_velocity;
	init_local_velocity (local_velocity, particle);
	
	ParticleVelocity global_velocity;
	init_global_velocity (global_velocity, particle);
	
	float old_factor = 1.0 - velocity_change_rate;
	particle.velocity *= old_factor;
	particle.velocity += local_velocity;
	particle.velocity += global_velocity;
}

void
ParticleSwarmOptimizer::update_local_best (Particle & particle)
{
	if (particle.result > particle.local_best_result) {
		particle.local_best_result = particle.result;
		particle.local_best_values = particle.values;
	}
}

void
ParticleSwarmOptimizer::update_global_best (Particle const & particle)
{
	if (particle.result > global_best_result) {
		LockGuard lg (global_best_mutex);
		global_best_result = particle.result;
		*global_best_values = particle.values;
	}
}

void
ParticleSwarmOptimizer::init_random_velocity (ParticleVelocity & velocity, unsigned size)
{
	velocity.resize (size);
	for (ParticleVelocity::iterator it = velocity.begin (); it != velocity.end (); ++it) {
		*it = rg.random_uniform_01 ();
	}
}

void
ParticleSwarmOptimizer::init_random_velocity_for_set (ParticleVelocity & velocity, ParameterSet const & set)
{
	velocity.resize (set.size ());
	ParameterSet::iterator set_it = set.begin ();
	for (ParticleVelocity::iterator v_it = velocity.begin (); v_it != velocity.end (); ++v_it, ++set_it) {
		Parameter const & param = *set_it->second;
		*v_it = initial_velocity * rg.random_uniform (param.min (), param.max ());
	}
}

void
ParticleSwarmOptimizer::init_local_velocity (ParticleVelocity & velocity, Particle & particle)
{
	float local_factor = (1.0 - global_velocity_weight) * velocity_change_rate;
	
	init_random_velocity (velocity, particle.values.size ());
	velocity *= ParticleVelocity (particle.values, particle.local_best_values);
	velocity *= local_factor;
}

void
ParticleSwarmOptimizer::init_global_velocity (ParticleVelocity & velocity, Particle & particle)
{
	float global_factor = global_velocity_weight * velocity_change_rate;
	
	init_random_velocity (velocity, particle.values.size ());
	{
		LockGuard lg (global_best_mutex);
		velocity *= ParticleVelocity (particle.values, *global_best_values);
	}
	velocity *= global_factor;
}

} // namespace ReverbTuner
