#ifndef REVERB_TUNER_EVALUATION_SCHEDULER_H
#define REVERB_TUNER_EVALUATION_SCHEDULER_H

namespace ReverbTuner {

class DataSource;
class EvaluationSet;

/** Base class for a evaluation scheduler
  */
class EvaluationScheduler
{
  public:
	EvaluationScheduler (DataSource const & data_source)
	  : data_source (data_source) {}
	virtual ~EvaluationScheduler () {}
	
	/** Initialize evaluators
	  * \tparam EvaluatorType type of evaluator to instantiate, must be subclass of Evaluator
	  * \param amount number of evaluators to instantiate
	  */
	template<typename EvaluatorType>
	virtual void alloc_resources (unsigned amount) = 0;
	
	/** Evaluates a set of paramters. Blocks until evaluation is finished.
	  */
	virtual void evaluate (EvaluationSet & set) = 0;
	
  protected:
	DataSource const & data_source;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_EVALUATION_SCHEDULER_H
