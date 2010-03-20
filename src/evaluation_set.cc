#include "reverbtuner/evaluation_set.h"

#include <boost/assign/ptr_list_inserter.hpp>

namespace ReverbTuner {

void
EvaluationSet::resize (unsigned new_size, ParameterInitializer initializer)
{
	long int size_diff = new_size - data.size();
	while (size_diff > 0) {
		boost::assign::ptr_push_back (data) (param_set);
		initializer (data.back ().parameters);
		--size_diff;
	}
	while (size_diff < 0) {
		data.pop_back ();
		++size_diff;
	}
}

bool
EvaluationSet::next_pair (ParameterValues *& parameters, EvaluationResult *& result) throw()
{
	if (current_item == data.end()) { return false; }
	DataContainer::iterator it = current_item++;
	parameters = &it->parameters;
	result = &it->result;
	return true;
}

} // namespace ReverbTuner
