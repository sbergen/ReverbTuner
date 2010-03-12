#ifndef REVERB_TUNER_EVOLUTIONARY_OPTIMIZER_H
#define REVERB_TUNER_EVOLUTIONARY_OPTIMIZER_H

#include <map>

#include "reverbtuner/evaluation_set.h"
#include "reverbtuner/parameter_modifier.h"

namespace ReverbTuner {

class DataSource;
class EvaluationScheduler;

class EvolutionaryOptimizer
{
  public:
	EvolutionaryOptimizer (DataSource const & data_source, EvaluationScheduler & scheduler);
	~EvolutionaryOptimizer ();

	void run ();
	
  private:
	void initialize_set ();
	
	// Parameter modification
	void randomize_all_values (ParameterValues & values);
	void mutate_one (ParameterValues & values);
	void cross_over_from_selected (ParameterValues & values);
	
	// Selection
	void results_into_map ();
	void select_best ();
	void select_random ();
	ParameterValues const * random_from_selected ();
	
  private: // Main data
	DataSource const & data_source;
	EvaluationScheduler & scheduler;
	
	EvaluationSet evaluation_set;
	ParameterModifier param_modifier;
	
  private: // intermediate data
	typedef std::map<float, ParameterValues const *> ResultMap;
	ResultMap all_results;
	ResultMap selected_results;
	
  private: // Algorithm parameters
	unsigned rounds;
	unsigned population_size;
	unsigned best_selection_size;
	unsigned random_selection_size;
	unsigned number_of_parents;
	
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_EVOLUTIONARY_OPTIMIZER_H
