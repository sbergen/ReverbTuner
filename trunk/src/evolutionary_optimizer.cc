#include "reverbtuner/evolutionary_optimizer.h"

#include "reverbtuner/evaluation_result.h"

namespace ReverbTuner {

EvolutionaryOptimizer::EvolutionaryOptimizer (DataSource const & data_source, EvaluationScheduler & scheduler)
  , data_source (data_source)
  , scheduler (scheduler)
  , population_size (20)
{
}

void
EvolutionaryOptimizer::run ()
{
	evaluation_set.resize (population_size);
	
}

EvolutionaryOptimizer::~EvolutionaryOptimizer ()
{

}

} // namespace ReverbTuner
