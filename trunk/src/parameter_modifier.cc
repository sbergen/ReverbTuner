#include "reverbtuner/parameter_modifier.h"

#include <ctime>

#include <boost/random/uniform_real.hpp>
#include <boost/random/triangle_distribution.hpp>
#include <boost/random/uniform_smallint.hpp>

#include "reverbtuner/parameter.h"
#include "reverbtuner/parameter_set.h"

#include <iostream>

namespace ReverbTuner {

ParameterModifier::ParameterModifier(double samplerate)
  : samplerate (samplerate)
  , rng (std::time (0))
  , uniform_generator (rng, uniform_dist)
{

}

void
ParameterModifier::randomize_uniform (float & value, Parameter const & param)
{
	boost::uniform_real<float> distribution (param.min (), param.max ());
	value = distribution (rng);
	make_valid (value, param);
}

void
ParameterModifier::randomize_triangular (float & value, Parameter const & param)
{
	boost::triangle_distribution<float> distribution (param.min (), param.def (), param.max ());
	value = distribution (uniform_generator);
	make_valid (value, param);
}

void
ParameterModifier::make_valid (float & value, Parameter const & param)
{
	clamp (value, param.min (), param.max ());
	switch (param.type ()) {
		case Parameter::TypeNormal:
			// Do nothing
			break;
		case Parameter::TypeInteger:
			value = static_cast<int> (value);
			break;
		case Parameter::TypeToggled:
			value = (value > 0.5);
			break;
		case Parameter::TypeSampleRate:
			value = value * samplerate;
			break;
	}
}

unsigned
ParameterModifier::random_with_max_value (unsigned max)
{
	boost::uniform_smallint<unsigned> distribution (0, max);
	return distribution (rng);
}

unsigned
ParameterModifier::random_index (ParameterSet const & set)
{
	return set.index_from_random_number (rng ());
}

} // namespace ReverbTuner
