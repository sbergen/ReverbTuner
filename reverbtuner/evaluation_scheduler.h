#ifndef REVERB_TUNER_EVALUATION_SCHEDULER_H
#define REVERB_TUNER_EVALUATION_SCHEDULER_H

namespace ReverbTuner {

class DataSource;
class EvaluationSet;

/** Base class for a evaluation scheduler, that handles both memory allocation and concurrency
  * \tparam EvaluatorType evaluator which is use, must be a subclass of \a EvaluationResources
  */
template<typename EvaluatorType>
class EvaluationScheduler
{
  public:
	EvaluationScheduler (DataSource const & data_source)
	  : data_source (data_source) {}
	virtual ~EvaluationScheduler () {}
	
	/** Allocates resources to run parameter evaluation concurrently
	  * The allocated resources should be usable across several calls to \a evaluate.
	  * \param concurrency number of evaluations to run concurrently
	  */
	virtual void alloc_resources (int concurrency) = 0;
	
	/** Evaluates a set of paramters. Blocks until evaluation is finished.
	  */
	virtual void evaluate (EvaluationSet & set) = 0;
	
  protected:
	DataSource const & data_source;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_EVALUATION_SCHEDULER_H
