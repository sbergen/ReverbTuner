#ifndef REVERB_TUNER_EVOLUTIONARY_OPTIMIZER_H
#define REVERB_TUNER_EVOLUTIONARY_OPTIMIZER_H

#include "reverbtuner/evaluation_set.h"

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
	DataSource const & data_source;
	EvaluationScheduler & scheduler;
	
	EvaluationSet evaluation_set;
	unsigned population_size;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_EVOLUTIONARY_OPTIMIZER_H
