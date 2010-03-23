#include "reverbtuner/parameter_modifier.h"

#include "reverbtuner/parameter.h"
#include "reverbtuner/parameter_set.h"
#include "reverbtuner/parameter_values.h"
#include "reverbtuner/random_generator.h"

#include <iostream>

namespace ReverbTuner {

ParameterModifier::ParameterModifier(double samplerate, RandomGenerator & rg)
  : samplerate (samplerate)
  , rg (rg)
{

}

void
ParameterModifier::randomize_uniform (float & value, Parameter const & param)
{
	value = rg.random_uniform (param.min (), param.max ());
}

void
ParameterModifier::randomize_triangular (float & value, Parameter const & param)
{
	value =  rg.random_triangular (param.min (), param.def (), param.max ());
}

void
ParameterModifier::randomize_all (ParameterValues & values, float uniform_probability)
{
	ParameterSet const & set = values.get_set ();
	for (ParameterSet::iterator it = set.begin (); it != set.end (); ++it) {
		if (rg.random_bool (uniform_probability)) {
			randomize_uniform (values[it->first], *it->second);
		} else {
			randomize_triangular (values[it->first], *it->second);
		}
	}
}

} // namespace ReverbTuner
