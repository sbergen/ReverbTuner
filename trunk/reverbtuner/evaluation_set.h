#ifndef REVERB_TUNER_EVALUATION_SET_H
#define REVERB_TUNER_EVALUATION_SET_H

#include <boost/ptr_container/ptr_list.hpp>
#include <boost/function.hpp>

#include "reverbtuner/evaluation_result.h"
#include "reverbtuner/parameter_values.h"

namespace ReverbTuner {

class ParameterSet;

/** Stores parameter-result-pairs for evaluation.
  * Data access is not thread safe.
  * Constness applies to ParameterValues
  */
class EvaluationSet
{
  public:
	EvaluationSet (ParameterSet const & param_set) : param_set (param_set) {}
	
	typedef boost::function<void (ParameterValues &)> ParameterInitializer;
	void resize (unsigned new_size, ParameterInitializer initializer = ParameterInitializer ());
	
	unsigned size() { return data.size(); }
	void go_to_first () const { current_item = data.begin(); }
	bool next_pair (ParameterValues const *& parameters, EvaluationResult *& result) const throw()
		{ return const_cast<EvaluationSet &> (*this).next_pair (const_cast<ParameterValues *&> (parameters), result); }
	bool next_pair (ParameterValues *& parameters, EvaluationResult *& result) throw();
	
  private:
	
	struct DataPair
	{
		DataPair (ParameterSet const & param_set)
		  : parameters (param_set) {}
		
		ParameterValues parameters;
		EvaluationResult result;
	};
	
	typedef boost::ptr_list<DataPair> DataContainer;
	
	ParameterSet const & param_set;
	mutable DataContainer data;
	mutable DataContainer::iterator current_item;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_EVALUATION_SET_H
