#include "reverbtuner/parameter.h"

namespace ReverbTuner {

Parameter::Parameter (Type type, float default_value, float min_value, float max_value)
  : type (type)
  , default_value (default_value)
  , min_value (min_value)
  , max_value (max_value)
{
}

float
Parameter::generate_random_value () const
{
	// TODO implement properly
	return default_value;
}

} // namespace ReverbTuner
