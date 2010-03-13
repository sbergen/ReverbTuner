#include "reverbtuner/evaluation_set.h"

#include <boost/assign/ptr_list_inserter.hpp>

namespace ReverbTuner {

void
EvaluationSet::resize (unsigned new_size)
{
	while (data.size() < new_size) {
		boost::assign::ptr_push_back (data) (param_set);
	}
	while (data.size() > new_size) {
		data.pop_back ();
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
