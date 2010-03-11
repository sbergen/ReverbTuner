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
	  * \param amount number of evaluators to instantiate
	  */
	virtual void alloc_resources (unsigned amount) = 0;
	
	/** Evaluates a set of paramters. Blocks until evaluation is finished.
	  */
	virtual void evaluate (EvaluationSet const & set) = 0;
	
  protected:
	DataSource const & data_source;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_EVALUATION_SCHEDULER_H
