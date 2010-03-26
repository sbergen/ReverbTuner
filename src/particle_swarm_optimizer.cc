#include "reverbtuner/particle_swarm_optimizer.h"

#include <cmath>

#include "reverbtuner/data_source.h"
#include "reverbtuner/evaluation_progress.h"
#include "reverbtuner/evaluation_scheduler.h"
#include "reverbtuner/parameter.h"
#include "reverbtuner/parameter_set.h"
#include "reverbtuner/parameter_values.h"
#include "reverbtuner/plugin.h"
#include "reverbtuner/random_generator.h"

#include <iostream>

namespace ReverbTuner {

ParticleSwarmOptimizer::ParticleSwarmOptimizer (DataSource const & data_source, EvaluationScheduler & scheduler, RandomGenerator & rg)
  : global_best_values (data_source.get_plugin()->get_parameters())
  , evaluation_set (data_source.get_plugin()->get_parameters())
  , param_modifier (data_source.get_samplerate(), rg)
  , data_source (data_source)
  , scheduler (scheduler)
  , rg (rg)
  , rounds (200)
  , population_size (150)
  , initial_velocity (0.0)
  , current_velocity_factor (1.0)
  , local_velocity_factor (1.9)
  , global_velocity_factor (1.9)
  , series_factor (0.92)
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
	params.reset (new ParameterValues (global_best_values));
	return true;
}

void
ParticleSwarmOptimizer::do_run ()
{
	ensure_population_size ();
	scheduler.evaluate (evaluation_set);
	
	for (unsigned i = 1; i < rounds; ++i) {
		if (progress->aborted()) { return; }
		
		initialize_min_velocity (i);
		progress->set_current_round (i + 1);
		ensure_population_size ();
		
		move_particles ();
		scheduler.evaluate (evaluation_set);
		
		std::cout << "Best for round: " << global_best_result << std::endl;
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
ParticleSwarmOptimizer::initialize_min_velocity (unsigned round)
{
	ParameterSet const & set = data_source.get_plugin()->get_parameters();
	round_max_velocity.resize (set.size ());
	
	float common_factor = std::pow (series_factor, round);
	
	ParameterSet::iterator param_it;
	ParticleVelocity::iterator vel_it = round_max_velocity.begin ();
	for (param_it = set.begin (); param_it != set.end (); ++param_it, ++vel_it) {
		float range = param_it->second->max () - param_it->second->min ();
		*vel_it = common_factor * range;
	}
}

void
ParticleSwarmOptimizer::move_particles ()
{
	for (SwarmEvaluationSet::iterator it = evaluation_set.begin (); it != evaluation_set.end (); ++it) {
		update_local_best (*it);
		update_global_best (*it);
		move_particle (*it);
	}
}

void
ParticleSwarmOptimizer::move_particle (Particle & particle)
{
	static const Particle * first_particle = &particle;
	
	update_particle_velocity (particle);
	limit_particle_velocity_to_bounds (particle);
	particle.values += particle.velocity;
	particle.values.limit_to_bounds ();
	
	if (&particle == first_particle) {
		visualize (particle);
	}
}

void
ParticleSwarmOptimizer::update_particle_velocity (Particle & particle)
{
	ParticleVelocity local_velocity;
	init_local_velocity (local_velocity, particle);
	
	ParticleVelocity global_velocity;
	init_global_velocity (global_velocity, particle);
	
	particle.velocity *= current_velocity_factor;
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
		global_best_values = particle.values;
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
	init_random_velocity (velocity, particle.values.size ());
	velocity *= ParticleVelocity (particle.values, particle.local_best_values);
	velocity *= local_velocity_factor;
}

void
ParticleSwarmOptimizer::init_global_velocity (ParticleVelocity & velocity, Particle & particle)
{
	init_random_velocity (velocity, particle.values.size ());
	{
		LockGuard lg (global_best_mutex);
		velocity *= ParticleVelocity (particle.values, global_best_values);
	}
	velocity *= global_velocity_factor;
}

void
ParticleSwarmOptimizer::limit_particle_velocity_to_bounds (Particle & particle)
{
	ParticleVelocity & velocity = particle.velocity;
	ParticleVelocity::iterator it, max_it = round_max_velocity.begin ();
	for (it = velocity.begin (); it != velocity.end (); ++it, ++max_it) {
		clamp (*it, -*max_it, *max_it);
	}
}

void
ParticleSwarmOptimizer::visualize (Particle & particle)
{
	ParameterSet const & set = particle.values.get_set ();
	for (ParameterValues::const_iterator it = particle.values.begin (); it != particle.values.end(); ++it) {
		Parameter const & param = set[it->first];
		
		float value = it->second;
		int percentage = 100 * (value - param.min()) / (param.max() - param.min());
		
		value = global_best_values[it->first];
		int best_percentage = 100 * (value - param.min()) / (param.max() - param.min());
		
		value = particle.local_best_values[it->first];
		int local_percentage = 100 * (value - param.min()) / (param.max() - param.min());
		
		for (int i = 0; i < 100; ++i) {
			if (i == best_percentage) {
				std::cout << ":";
			} else if (i == local_percentage) {
				std::cout << ".";
			} else if (i == percentage) {
				std::cout << "|";
			} else {
				std::cout << "-";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

} // namespace ReverbTuner
