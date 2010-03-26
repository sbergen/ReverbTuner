#ifndef REVERB_TUNER_PARTICLE_SWARM_OPTIMIZER_H
#define REVERB_TUNER_PARTICLE_SWARM_OPTIMIZER_H

#include <boost/ptr_container/ptr_map.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>

#include "reverbtuner/evaluation_result.h"
#include "reverbtuner/parameter_modifier.h"
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
	void initialize_global_best ();
	
	void move_particles ();
	void move_particle (Particle & particle);
	void update_particle_velocity (Particle & particle);
	void update_local_best (Particle & particle);
	void update_global_best (Particle const & particle);
	
	void init_random_velocity (ParticleVelocity & velocity, unsigned size);
	void init_random_velocity_for_set (ParticleVelocity & velocity, ParameterSet const & set);
	void init_local_velocity (ParticleVelocity & velocity, Particle & particle);
	void init_global_velocity (ParticleVelocity & velocity, Particle & particle);
	
	ScopedParameterValuesPtr global_best_values;
	EvaluationResult global_best_result;
	boost::mutex global_best_mutex;
	
	SharedEvaluationProgressPtr progress;
	
	SwarmEvaluationSet evaluation_set;
	ParameterModifier param_modifier;
	
	DataSource const & data_source;
	EvaluationScheduler & scheduler;
	RandomGenerator & rg;
	
  private: // Algorithm parameters
	unsigned rounds;
	unsigned population_size;
	
	float initial_velocity;
	float velocity_change_rate;
	float global_velocity_weight;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_PARTICLE_SWARM_OPTIMIZER_H
