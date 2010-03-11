#include "reverbtuner/evaluation_set.h"

namespace ReverbTuner {

EvaluationSet::EvaluationSet (ParameterSet const & param_set)
  : param_set (param_set)
{
}

EvaluationSet::~EvaluationSet ()
{
}

void
EvaluationSet::resize (unsigned new_size)
{
	LockGuard lock (data_mutex);
	
	while (data.size() < new_size) {
		data.push_back (new DataPair (param_set));
	}
	
	while (data.size() > new_size) {
		data.pop_back ();
	}
}

void
EvaluationSet::go_to_first () const
{
	LockGuard lock (data_mutex);
	current_item = data.begin();
}

bool
EvaluationSet::next_pair (ParameterValues const *& parameters, EvaluationResult *& result) const
{
	return const_cast<EvaluationSet &> (*this).next_pair (const_cast<ParameterValues *&> (parameters), result);
}

bool
EvaluationSet::next_pair (ParameterValues *& parameters, EvaluationResult *& result)
{
	if (current_item == data.end()) { return false; }
	
	LockGuard lock (data_mutex);
	DataContainer::iterator it = current_item++;
	parameters = &it->parameters;
	result = &it->result;
	return true;
}

} // namespace ReverbTuner
