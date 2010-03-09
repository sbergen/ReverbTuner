#ifndef REVERB_TUNER_EVALUATOR_H
#define REVERB_TUNER_EVALUATOR_H

namespace ReverbTuner {

class DataSource;
class ParameterValues;
class EvaluationResult;

/// Base class for all Parameter evaluators.
class Evaluator
{
  public:
	Evaluator (DataSource const & data_source)
	  : data_source (data_source) {}
	
	virtual ~Evaluator() {}

	/** Evaluates the given parameter set for the data source set in constructor.
	  * \param parameters The parameter set to evaluate
	  * \param result Result stucutre into which the result is stored
	  */
	virtual void evaluate_parameters (ParameterValues const & parameters, EvaluationResult const & result) = 0;

  protected:
	DataSource const & data_source;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_EVALUATOR_H
