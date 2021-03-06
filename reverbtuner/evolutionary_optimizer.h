#ifndef REVERB_TUNER_EVOLUTIONARY_OPTIMIZER_H
#define REVERB_TUNER_EVOLUTIONARY_OPTIMIZER_H

#include <boost/ptr_container/ptr_map.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>

#include "reverbtuner/basic_evaluation_set.h"
#include "reverbtuner/parameter_modifier.h"
#include "reverbtuner/types.h"

namespace ReverbTuner {

class EvolutionaryOptimizer
{
  public:
	EvolutionaryOptimizer (DataSource const & data_source, EvaluationScheduler & scheduler, RandomGenerator & rg);
	~EvolutionaryOptimizer ();

	/// Runs evaluation in new thread. Progress information is shared via the structure
	void run (SharedEvaluationProgressPtr progress_);
	bool get_best_params (ScopedParameterValuesPtr & params);
	
  private:
	void do_run ();
	void ensure_population_size ();
	
	// Parameter modification
	void mutate (ParameterValues & values);
	void cross_over_from_selected (ParameterValues & values);
	
	// Selection
	void results_into_map ();
	void store_best_result ();
	void select_best ();
	void select_random ();
	ParameterValues const * random_from_selected ();
	
	// Reproduction
	void reproduce_from_selected ();
	
  private: // Main data
	DataSource const & data_source;
	EvaluationScheduler & scheduler;
	RandomGenerator & rg;
	
	BasicEvaluationSet evaluation_set;
	ParameterModifier param_modifier;
	
	float best_value;
	ScopedParameterValuesPtr best_params;
	boost::mutex params_mutex;
	
	SharedEvaluationProgressPtr progress;
	
  private: // intermediate data
	// We need to take a copy of selected values
	typedef boost::ptr_multimap<float, ParameterValues> ResultPtrMap;
	ResultPtrMap selected_results;
	
	typedef std::multimap<float, ParameterValues const *> ResultMap;
	ResultMap all_results;
	
  private: // Algorithm parameters
	unsigned rounds;
	unsigned population_size;
	unsigned best_selection_size;
	unsigned random_selection_size;
	unsigned number_of_parents;
	float set_mutation_probability;
	float parameter_mutation_probability;
	float uniform_probability;
	
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_EVOLUTIONARY_OPTIMIZER_H
