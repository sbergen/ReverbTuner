#ifndef REVERB_TUNER_PARTICLE_SWARM_OPTIMIZER_H
#define REVERB_TUNER_PARTICLE_SWARM_OPTIMIZER_H

#include <boost/ptr_container/ptr_map.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>

#include "reverbtuner/evaluation_result.h"
#include "reverbtuner/parameter_modifier.h"
#include "reverbtuner/particle_velocity.h"
#include "reverbtuner/swarm_evaluation_set.h"
#include "reverbtuner/types.h"

namespace ReverbTuner {

class ParticleSwarmOptimizer
{
  public:
	ParticleSwarmOptimizer (DataSource const & data_source, EvaluationScheduler & scheduler, RandomGenerator & rg);
	~ParticleSwarmOptimizer ();

	/// Runs evaluation in new thread. Progress information is shared via the structure
	void run (SharedEvaluationProgressPtr progress_);
	bool get_best_params (ScopedParameterValuesPtr & params);
	
  private:
	
	typedef SwarmEvaluationSet::Particle Particle;
	
	void do_run ();
	
	void ensure_population_size ();
	void initialize_min_velocity (unsigned round);
	
	void move_particles ();
	void move_particle (Particle & particle);
	void update_particle_velocity (Particle & particle);
	void update_local_best (Particle & particle);
	void update_global_best (Particle const & particle);
	
	void init_random_velocity (ParticleVelocity & velocity, unsigned size);
	void init_random_velocity_for_set (ParticleVelocity & velocity, ParameterSet const & set);
	void init_local_velocity (ParticleVelocity & velocity, Particle & particle);
	void init_global_velocity (ParticleVelocity & velocity, Particle & particle);
	
	void limit_particle_velocity_to_bounds (Particle & particle);
	
	ParameterValues global_best_values;
	EvaluationResult global_best_result;
	boost::mutex global_best_mutex;
	
	SharedEvaluationProgressPtr progress;
	
	SwarmEvaluationSet evaluation_set;
	ParameterModifier param_modifier;
	
	DataSource const & data_source;
	EvaluationScheduler & scheduler;
	RandomGenerator & rg;
	
	ParticleVelocity round_max_velocity;
	
  private: // Algorithm parameters
	unsigned rounds;
	unsigned population_size;
	
	float initial_velocity;
	float current_velocity_factor;
	float local_velocity_factor;
	float global_velocity_factor;
	
	float series_factor;
	
  private: // Stuff not really related to algorithm
	void visualize (Particle & particle);
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_PARTICLE_SWARM_OPTIMIZER_H
