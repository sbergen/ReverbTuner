#ifndef REVERB_TUNER_SINGLE_THREADED_SCHEDULER_H
#define REVERB_TUNER_SINGLE_THREADED_SCHEDULER_H

#include <boost/shared_ptr.hpp>

#include "evaluation_scheduler.h"

namespace ReverbTuner {

template<typename EvaluatorType>
class SingleThreadedScheduler : public EvaluationScheduler<EvaluatorType>
{
	typedef EvaluationScheduler<EvaluatorType> Parent;

  public:
	SingleThreadedScheduler (DataSource const & data_source)
	  : Parent (data_source) {}
	~SingleThreadedScheduler () {}
	
	void alloc_resources (unsigned /*amount*/)
	{
		evaluator.reset (new EvaluatorType (Parent::data_source));
	}
	
	void evaluate (EvaluationSet const & set)
	{
		ParameterValues const * parameters;
		EvaluationResult * result;
		
		set.go_to_first ();
		while (set.next_pair (parameters, result)) {
			evaluator->evaluate_parameters (*parameters, *result);
		}
	}
	
  private:
	typedef boost::shared_ptr<EvaluatorType> EvaluatorPtr;
	EvaluatorPtr evaluator;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_SINGLE_THREADED_SCHEDULER_H
