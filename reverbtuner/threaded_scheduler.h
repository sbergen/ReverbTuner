#ifndef REVERB_TUNER_THREADED_SCHEDULER_H
#define REVERB_TUNER_THREADED_SCHEDULER_H

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/thread.hpp>

#include "evaluation_scheduler.h"
#include "evaluation_set.h"
#include "parameter_values.h"

#include <iostream>

namespace ReverbTuner {

template<typename EvaluatorType>
class ThreadedScheduler : public EvaluationScheduler
{
  public:
	ThreadedScheduler (DataSource const & data_source)
	  : EvaluationScheduler (data_source) {}
	~ThreadedScheduler () {}
	
	void alloc_resources (unsigned amount = boost::thread::hardware_concurrency())
	{
		evaluators.clear ();
		for (unsigned i = 0; i < amount; ++i) {
			boost::assign::ptr_push_back (evaluators) (data_source);
		}
	}
	
	void evaluate (EvaluationSet const & set)
	{
		set.go_to_first ();
		
		// Wait for evaluator threads + this thread
		SyncedData data (set, evaluators.size () + 1);
		for (typename EvaluatorContainer::iterator it = evaluators.begin (); it != evaluators.end (); ++it) {
			boost::thread (start_evaluator, boost::ref (data), boost::ref (*it));
		}
		
		std::cout << "evaluate before wait" << std::endl;
		data.barrier.wait ();
		std::cout << "evaluate after wait" << std::endl;
	}
	
  private:
	
	// Synced data struct shared by threads
	struct SyncedData
	{
		SyncedData (EvaluationSet const & set, unsigned barrier_count)
		  : set (set), barrier (barrier_count) {}
		
		EvaluationSet const & set;
		boost::mutex   mutex;
		boost::barrier barrier;
	};
	
	
	static void start_evaluator (SyncedData & data, EvaluatorType & evaluator)
	{
		ParameterValues const * parameters;
		EvaluationResult * result;
		
		while (true) {
			data.mutex.lock();
			bool have_data = data.set.next_pair (parameters, result);
			data.mutex.unlock();
			
			if (!have_data) { break; }
			evaluator.evaluate_parameters (*parameters, *result);
		}
		
		std::cout << "start_evaluator before wait" << std::endl;
		data.barrier.wait ();
		std::cout << "start_evaluator after wait" << std::endl;
	}
	
	typedef boost::ptr_vector<EvaluatorType> EvaluatorContainer;
	EvaluatorContainer evaluators;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_THREADED_SCHEDULER_H
