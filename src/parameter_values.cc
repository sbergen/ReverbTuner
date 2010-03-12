#include "reverbtuner/parameter_values.h"

#include "reverbtuner/parameter_set.h"

namespace ReverbTuner {

ParameterValues::ParameterValues (ParameterSet const & set)
  : set (set)
{
	for (ParameterSet::Container::const_iterator it = set.begin(); it != set.end(); ++it) {
		values[it->first] = it->second->def ();
	}
}

ParameterValues::ParameterValues (ParameterValues const & other)
  : set (other.set)
{
	*this = other;
}

ParameterValues &
ParameterValues::operator= (ParameterValues const & other)
{
	values.clear ();
	for (Values::const_iterator it = other.values.begin(); it != other.values.end(); ++it) {
		values[it->first] = it->second;
	}
	return *this;
}
	
float &
ParameterValues::operator[] (unsigned index)
{
	return values[index];
}

float
ParameterValues::operator[] (unsigned index) const
{
	return const_cast<ParameterValues &>(*this)[index];
}

float &
ParameterValues::operator[] (Parameter const & param)
{
	return values[set.index_of_parameter (&param)];
}

float
ParameterValues::operator[] (Parameter const & param) const
{
	return const_cast<ParameterValues &>(*this)[param];
}

} // namespace ReverbTuner
