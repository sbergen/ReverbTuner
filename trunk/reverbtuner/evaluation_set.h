#ifndef REVERB_TUNER_EVALUATION_SET_H
#define REVERB_TUNER_EVALUATION_SET_H

#include <boost/ptr_container/ptr_list.hpp>
#include <boost/thread.hpp>

#include "reverbtuner/evaluation_result.h"
#include "reverbtuner/parameter_values.h"

namespace ReverbTuner {

class ParameterSet;

/** Stores parameter-result-pairs for evaluation.
  * Acess to data is thread safe.
  * Constness applies to ParameterValues
  */
class EvaluationSet
{
  public:
	EvaluationSet (ParameterSet const & param_set);
	~EvaluationSet ();

	bool resize (unsigned new_size);
	
	void go_to_first () const;
	
	bool next_pair (ParameterValues const *& parameters, EvaluationResult *& result) const;
	bool next_pair (ParameterValues *& parameters, EvaluationResult *& result);
	
  private:
	struct DataPair
	{
		DataPair (ParameterSet const & param_set)
		  : parameters (param_set) {}
		
		ParameterValues parameters;
		EvaluationResult result;
	};
	
	typedef boost::ptr_list<DataPair> DataContainer;
	typedef boost::mutex Mutex;
	typedef boost::lock_guard<Mutex> LockGuard;
	
	ParameterSet const & param_set;
	mutable DataContainer data;
	mutable DataContainer::iterator current_item;
	mutable Mutex data_mutex;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_EVALUATION_SET_H
