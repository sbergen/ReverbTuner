#include "reverbtuner/parameter.h"

#include <iostream>

namespace ReverbTuner {

Parameter::Parameter (Type type, float default_value, float min_value, float max_value)
  : type (type)
  , default_value (default_value)
  , min_value (min_value)
  , max_value (max_value)
{
	//std::cerr << "Parameter (" << default_value << ", " << min_value << ", " << max_value << ", " << type << ")" << std::endl;
}

} // namespace ReverbTuner
