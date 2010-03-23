#include "reverbtuner/parameter_values.h"

#include "reverbtuner/parameter_set.h"
#include "reverbtuner/utils.h"

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
	for (Values::const_iterator it = other.values.begin(); it != other.values.end(); ++it) {
		values[it->first] = it->second;
	}
	return *this;
}

void
ParameterValues::apply_validated_values (ParameterValues const & other, double samplerate)
{
	for (Values::const_iterator it = other.values.begin(); it != other.values.end(); ++it) {
		unsigned index = it->first;
		values[index] = validate_value (it->second, set[index], samplerate);
	}
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

float
ParameterValues::validate_value (float value, Parameter const & param, double samplerate)
{
	clamp (value, param.min (), param.max ());
	switch (param.type ()) {
		case Parameter::TypeNormal:
			// Do nothing
			break;
		case Parameter::TypeInteger:
			// Proper rounding with + 0.5
			value = static_cast<int> (value + 0.5);
			break;
		case Parameter::TypeToggled:
			value = (value > 0.5);
			break;
		case Parameter::TypeSampleRate:
			value = value * samplerate;
			break;
	}
	return value;
}

std::ostream & operator<< (std::ostream & stream, ParameterValues const & values)
{
	ParameterSet const & set = values.get_set ();
	for (ParameterValues::const_iterator it = values.begin (); it != values.end(); ++it) {
		stream << set[it->first].name () << ": " << it->second << std::endl;
	}
	return stream;
}

} // namespace ReverbTuner
