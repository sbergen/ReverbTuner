#ifndef REVERB_TUNER_EVALUATION_SET_H
#define REVERB_TUNER_EVALUATION_SET_H

#include <boost/ptr_container/ptr_list.hpp>
#include <boost/thread/mutex.hpp>

namespace ReverbTuner {

class ParameterSet;
class EvaluationResult;

/** Stores parameter-result-pairs for evaluation.
  * Acess to data is thread safe.
  * Results are considered mutable
  */
class EvaluationSet
{
  public:
	EvaluationSet ();
	~EvaluationSet ();

	bool resize (unsigned new_size);
	
	bool first_pair (ParameterSet const * parameters, EvaluationResult * result) const;
	bool first_pair (ParameterSet * parameters, EvaluationResult * result);
	
	bool next_pair (ParameterSet const * parameters, EvaluationResult * result) const;
	bool next_pair (ParameterSet * parameters, EvaluationResult * result);
	
  private:
	struct DataPair
	{
		ParameterSet parameters;
		EvaluationResult result
	};
	
	typedef boost::ptr_list<DataPair> DataContainer;
	
	boost::mutex data_mutex;
	DataContainer data;
	DataContainer::iterator current_item;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_EVALUATION_SET_H
