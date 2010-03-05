#ifndef REVERB_TUNER_EVALUATOR_H
#define REVERB_TUNER_EVALUATOR_H

namespace ReverbTuner {

class DataSource;
class ParameterSet;

class EvaluationResult;
class EvaluationResources;

/// Base class for all Parameter evaluators.
class Evaluator
{
  public:
	Evaluator (DataSource const & data_source, EvaluationResources & resources)
	  : data_source (data_source), resources (resources) {}
	virtual ~Evaluator() {}
	
	/** Evaluates the given parameter set for the data source set in constructor.
	  * \param parameters The parameter set to evaluate
	  * \param result Result stucutre into which the result is stored
	  */
	virtual void evaluate_parameters (ParameterSet const & parameters, EvaluationResult const & result) = 0;
	
	/** Allocate a resource structure that holds all memory needed by the evaluation function.
	  * This structure must be big enouhg for \a evaluate_parameters to complete with a specific DataSource.
	  * \param data_source Data source with which \a evaluate_parameters must work
	  * \return Allocated resource structure, ownership is transferred to caller
	  */
	virtual static EvaluationResources * alloc_resources (DataSource const & data_source) = 0;

  protected:
	DataSource const & data_source;
	EvaluationResources & resources;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_EVALUATOR_H