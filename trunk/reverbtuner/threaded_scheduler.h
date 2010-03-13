#ifndef REVERB_TUNER_THREADED_SCHEDULER_H
#define REVERB_TUNER_THREADED_SCHEDULER_H

#include <list>
#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include "evaluation_scheduler.h"
#include "evaluation_set.h"
#include "parameter_values.h"

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
			evaluators.push_back (EvaluatorPtr (new EvaluatorType (data_source)));
		}
	}
	
	void evaluate (EvaluationSet const & set)
	{
		set.go_to_first ();
		
		// Wait for evaluator threads + this thread
		SyncedDataPtr data (new SyncedData (set, evaluators.size () + 1));
		for (typename EvaluatorContainer::iterator it = evaluators.begin (); it != evaluators.end (); ++it) {
			boost::thread (start_evaluator, data, *it);
		}
		
		data->barrier.wait ();
	}
	
  private:
	
	// Synced data struct shared by threads
	struct SyncedData : private boost::noncopyable
	{
		SyncedData (EvaluationSet const & set, unsigned barrier_count)
		  : set (set), barrier (barrier_count) { }
		
		EvaluationSet const & set;
		boost::mutex   mutex;
		boost::barrier barrier;
	};
	
	typedef boost::shared_ptr<EvaluatorType> EvaluatorPtr;
	typedef boost::shared_ptr<SyncedData> SyncedDataPtr;
	
	
	static void start_evaluator (SyncedDataPtr data, EvaluatorPtr evaluator)
	{
		ParameterValues const * parameters;
		EvaluationResult * result;
		
		while (true) {
			data->mutex.lock();
			bool have_data = data->set.next_pair (parameters, result);
			data->mutex.unlock();
			
			if (!have_data) { break; }
			evaluator->evaluate_parameters (*parameters, *result);
		}
		
		data->barrier.wait ();
	}
	
	typedef std::list<EvaluatorPtr> EvaluatorContainer;
	EvaluatorContainer evaluators;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_THREADED_SCHEDULER_H
