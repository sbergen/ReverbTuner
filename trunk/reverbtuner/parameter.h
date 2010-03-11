#ifndef REVERB_TUNER_PARAMETER_H
#define REVERB_TUNER_PARAMETER_H

#include "utils.h"

namespace ReverbTuner {

class Parameter
{
  public:
	
	enum Type {
		TypeNormal,
		TypeInteger,
		TypeSampleRate,
		TypeToggled
	};
	
	Parameter (Type type, float default_value, float min_value, float max_value)
	  : type (type)
	  , min_value (min_value)
	  , max_value (max_value)
	  , default_value (default_value)
	{}
	
	void make_valid (float & value) const { clamp (value, min_value, max_value); }
	
	Type get_type () const { return type; }
	float get_minimum () const { return min_value; }
	float get_maximum () const { return max_value; }
	float get_default () const { return default_value; }
	
  private:
	
	Type  type;
	float min_value;
	float max_value;
	float default_value;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_PARAMETER_H
