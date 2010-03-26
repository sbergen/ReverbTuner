#ifndef REVERB_TUNER_SWARM_EVALUATION_SET_H
#define REVERB_TUNER_SWARM_EVALUATION_SET_H

#include <map>

#include <boost/ptr_container/ptr_list.hpp>
#include <boost/function.hpp>

#include "reverbtuner/evaluation_set.h"
#include "reverbtuner/evaluation_result.h"
#include "reverbtuner/parameter_values.h"
#include "reverbtuner/particle_velocity.h"

namespace ReverbTuner {

class ParameterSet;

/** Stores parameter-result-pairs for evaluation.
  * Data access is not thread safe.
  * Constness applies to ParameterValues
  */
class SwarmEvaluationSet : public EvaluationSet
{
  public: // EvaluationSet interface
	SwarmEvaluationSet (ParameterSet const & param_set)
	  : param_set (param_set) {}
	
	typedef boost::function<void (ParticleVelocity &, ParameterSet const &)> VelocityInitializer;
	void resize (unsigned new_size, ParameterInitializer parameter_initializer, VelocityInitializer velocity_initializer);
	
	void go_to_first () const { current_item = data.begin(); }
	bool next_pair (ParameterValues *& parameters, EvaluationResult *& result) throw();
	using EvaluationSet::next_pair;
	
  public: // Swarm interface
	
	// Contains only particle data.
	// Operations on particles are performed in ParticleSwarmOptimizer
	struct Particle
	{
		Particle (ParameterSet const & param_set)
		  : values (param_set), local_best_values (param_set) { }
		
		// Evaluation data
		ParameterValues values;
		EvaluationResult result;
		
		// Particle data
		ParameterValues local_best_values;
		EvaluationResult local_best_result;
		
		ParticleVelocity velocity;
	};
	
	typedef boost::ptr_list<Particle> DataContainer;
	typedef DataContainer::iterator iterator;

	iterator begin() { return data.begin (); }
	iterator end() { return data.end (); }

  private:
	ParameterSet const & param_set;
	mutable DataContainer data;
	mutable DataContainer::iterator current_item;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_SWARM_EVALUATION_SET_H
