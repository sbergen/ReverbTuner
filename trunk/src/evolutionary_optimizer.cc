#include "reverbtuner/evolutionary_optimizer.h"

#include <algorithm>
#include <limits>
#include <set>

#include <boost/assign/ptr_map_inserter.hpp>
#include <boost/thread.hpp>
#include <boost/function.hpp>

#include "reverbtuner/data_source.h"
#include "reverbtuner/evaluation_progress.h"
#include "reverbtuner/evaluation_result.h"
#include "reverbtuner/evaluation_scheduler.h"
#include "reverbtuner/plugin.h"
#include "reverbtuner/parameter_set.h"
#include "reverbtuner/parameter_values.h"
#include "reverbtuner/utils.h"
#include "reverbtuner/random_generator.h"

#include <iostream>

namespace ReverbTuner {

EvolutionaryOptimizer::EvolutionaryOptimizer (DataSource const & data_source, EvaluationScheduler & scheduler, RandomGenerator & rg)
  : data_source (data_source)
  , scheduler (scheduler)
  , rg (rg)
  , evaluation_set (data_source.get_plugin()->get_parameters())
  , param_modifier (data_source.get_samplerate(), rg)
  , best_value (-std::numeric_limits<float>::infinity())
//  , rounds (10)
//  , population_size (200)
  , rounds (100)
  , population_size (200)
  
  , best_selection_size (8)
  , random_selection_size (3)
  , number_of_parents (2)
  , maximum_mutations (2)
{
}

EvolutionaryOptimizer::~EvolutionaryOptimizer ()
{
	
}

void
EvolutionaryOptimizer::run (boost::shared_ptr<EvaluationProgress> progress_)
{
	progress = progress_;
	progress->set_total_rounds (rounds);
	boost::thread (boost::bind (&EvolutionaryOptimizer::do_run, this));
}

bool
EvolutionaryOptimizer::get_best_params (ScopedParameterValuesPtr & result)
{
	LockGuard lg (params_mutex);
	if (!best_params) { return false; }
	result.reset (new ParameterValues (*best_params));
	return true;
}

void
EvolutionaryOptimizer::do_run ()
{
	initialize_set ();
	
	for (unsigned i = 0; i < rounds; ++i) {
		if (progress->aborted()) { return; }
		
		progress->set_current_round (i + 1);
		scheduler.evaluate (evaluation_set);
		results_into_map ();
		store_best_result ();
		select_best ();
		select_random ();
		reproduce_from_selected ();
	}
	
	progress->set_done ();
	std::cout << *best_params;
}

void
EvolutionaryOptimizer::initialize_set ()
{
	evaluation_set.resize (population_size);
	
	ParameterValues * parameters;
	EvaluationResult * result;
	evaluation_set.go_to_first();
	
	// Skip one set of parameters and leave them to defaults
	evaluation_set.next_pair (parameters, result);
	
	while (evaluation_set.next_pair (parameters, result)) {
		randomize_all_values (*parameters);
	}
}

void
EvolutionaryOptimizer::randomize_all_values (ParameterValues & values)
{
	bool const triangular = rg.random_bool ();
	ParameterSet const & set = values.get_set ();
	for (ParameterSet::iterator it = set.begin (); it != set.end (); ++it) {
		if (triangular) {
			param_modifier.randomize_triangular (values[it->first], *it->second);
		} else {
			param_modifier.randomize_uniform (values[it->first], *it->second);
		}
	}
}

void
EvolutionaryOptimizer::mutate (ParameterValues & values)
{
	unsigned mutations = rg.random_less_than (maximum_mutations + 1);
	for (unsigned i = 0; i < mutations; ++i) {
		mutate_one (values);
	}
}

void
EvolutionaryOptimizer::mutate_one (ParameterValues & values)
{
	unsigned index = values.get_set ().random_index (rg);
	param_modifier.randomize_uniform (values[index], values.get_set ()[index]);
}

void
EvolutionaryOptimizer::cross_over_from_selected (ParameterValues & values)
{
	// Select random parents
	std::set<ParameterValues const *> parents;
	while (parents.size() < number_of_parents) {
		parents.insert (random_from_selected ());
	}
	
	// Set values
	for (ParameterValues::iterator it = values.begin(); it != values.end(); ++it) {
		unsigned index = it->first;
		values[index] = (**random_from_container (parents, rg))[index];
	}
}

void
EvolutionaryOptimizer::results_into_map ()
{
	all_results.clear ();
	selected_results.clear();
	
	ParameterValues const * parameters;
	EvaluationResult * result;
	
	evaluation_set.go_to_first ();
	while (evaluation_set.next_pair (parameters, result)) {
		all_results[*result] = parameters;
	}
}

void
EvolutionaryOptimizer::store_best_result ()
{
	ResultMap::const_iterator best_it = --all_results.end();
	float this_time_best = best_it->first;
	if (this_time_best > best_value) {
		best_value = this_time_best;
		LockGuard lg (params_mutex);
		best_params.reset (new ParameterValues (*best_it->second));
	}
	
	progress->set_best_result (best_value);
	std::cout << "Best for round: " << this_time_best << std::endl;
}

void
EvolutionaryOptimizer::select_best ()
{
	for (unsigned i = 0; i < best_selection_size; ++i) {
		ResultMap::iterator it = --all_results.end ();
		boost::assign::ptr_map_insert (selected_results) (it->first, *it->second);
		all_results.erase (it);
	}
}

void
EvolutionaryOptimizer::select_random ()
{
	for (unsigned i = 0; i < random_selection_size; ++i) {
		ResultMap::iterator it = random_from_container (all_results, rg);
		boost::assign::ptr_map_insert (selected_results) (it->first, *it->second);
		all_results.erase (it);
	}
}

ParameterValues const *
EvolutionaryOptimizer::random_from_selected ()
{
	return random_from_container (selected_results, rg)->second;
}

void
EvolutionaryOptimizer::reproduce_from_selected ()
{
	ParameterValues * parameters;
	EvaluationResult * result;
	
	evaluation_set.go_to_first();
	while (evaluation_set.next_pair (parameters, result)) {
		cross_over_from_selected (*parameters);
		mutate (*parameters);
	}
}


} // namespace ReverbTuner
