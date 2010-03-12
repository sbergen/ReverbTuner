#include "reverbtuner/parameter_modifier.h"

#include "reverbtuner/parameter.h"
#include "reverbtuner/parameter_set.h"
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
	make_valid (value, param);
}

void
ParameterModifier::randomize_triangular (float & value, Parameter const & param)
{
	value =  rg.random_triangular (param.min (), param.def (), param.max ());
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
}


} // namespace ReverbTuner
