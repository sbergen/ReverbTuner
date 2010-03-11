#include "reverbtuner/parameter_modifier.h"

#include <ctime>

#include <boost/random/uniform_real.hpp>
#include <boost/random/triangle_distribution.hpp>

#include "reverbtuner/parameter.h"

namespace ReverbTuner {

ParameterModifier::ParameterModifier(double samplerate)
  : samplerate (samplerate)
  , rng (std::time (0))
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
	value = distribution (rng);
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

} // namespace ReverbTuner
