#include "reverbtuner/swarm_evaluation_set.h"

#include <boost/assign/ptr_list_inserter.hpp>

namespace ReverbTuner {

void
SwarmEvaluationSet::resize (unsigned new_size, ParameterInitializer parameter_initializer, VelocityInitializer velocity_initializer)
{
	long int size_diff = new_size - data.size();
	while (size_diff > 0) {
		boost::assign::ptr_push_back (data) (param_set);
		parameter_initializer (data.back ().values);
		velocity_initializer (data.back ().velocity, param_set);
		--size_diff;
	}
	while (size_diff < 0) {
		data.pop_back ();
		++size_diff;
	}
}

bool
SwarmEvaluationSet::next_pair (ParameterValues *& parameters, EvaluationResult *& result) throw()
{
	if (current_item == data.end()) { return false; }
	DataContainer::iterator it = current_item++;
	parameters = &it->values;
	result = &it->result;
	return true;
}

} // namespace ReverbTuner
