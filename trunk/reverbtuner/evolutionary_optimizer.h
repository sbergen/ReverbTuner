#ifndef REVERB_TUNER_EVOLUTIONARY_OPTIMIZER_H
#define REVERB_TUNER_EVOLUTIONARY_OPTIMIZER_H

#include <boost/ptr_container/ptr_map.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>

#include "reverbtuner/evaluation_set.h"
#include "reverbtuner/parameter_modifier.h"
#include "reverbtuner/types.h"

namespace ReverbTuner {

class DataSource;
class EvaluationScheduler;
class ParameterValues;
class EvaluationProgress;

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
	void initialize_set ();
	
	// Parameter modification
	void randomize_all_values (ParameterValues & values);
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
	
	EvaluationSet evaluation_set;
	ParameterModifier param_modifier;
	
	float best_value;
	boost::scoped_ptr<ParameterValues> best_params;
	boost::mutex params_mutex;
	
	boost::shared_ptr<EvaluationProgress> progress;
	
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
	float mutation_probability;
	float uniform_probability;
	
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_EVOLUTIONARY_OPTIMIZER_H
