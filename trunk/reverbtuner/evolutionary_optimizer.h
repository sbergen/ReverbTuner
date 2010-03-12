#ifndef REVERB_TUNER_EVOLUTIONARY_OPTIMIZER_H
#define REVERB_TUNER_EVOLUTIONARY_OPTIMIZER_H

#include <boost/ptr_container/ptr_map.hpp>

#include "reverbtuner/evaluation_set.h"
#include "reverbtuner/parameter_modifier.h"

namespace ReverbTuner {

class DataSource;
class EvaluationScheduler;

class EvolutionaryOptimizer
{
  public:
	EvolutionaryOptimizer (DataSource const & data_source, EvaluationScheduler & scheduler, RandomGenerator & rg);
	~EvolutionaryOptimizer ();

	void run ();
	
  private:
	void initialize_set ();
	
	// Parameter modification
	void randomize_all_values (ParameterValues & values);
	void mutate (ParameterValues & values);
	void mutate_one (ParameterValues & values);
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
	
	EvaluationSet evaluation_set;
	ParameterModifier param_modifier;
	
	float best_value;
	
  private: // intermediate data
	// We need to take a copy of selected values
	typedef boost::ptr_map<float, ParameterValues> ResultPtrMap;
	ResultPtrMap selected_results;
	
	typedef std::map<float, ParameterValues const *> ResultMap;
	ResultMap all_results;
	
  private: // Algorithm parameters
	unsigned rounds;
	unsigned population_size;
	unsigned best_selection_size;
	unsigned random_selection_size;
	unsigned number_of_parents;
	unsigned maximum_mutations;
	
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_EVOLUTIONARY_OPTIMIZER_H
