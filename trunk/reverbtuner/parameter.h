#ifndef REVERB_TUNER_PARAMETER_H
#define REVERB_TUNER_PARAMETER_H

#include <vector>

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
	
	Parameter (Type type, float default_value, float min_value, float max_value);
	
	Type get_type () const { return type; }
	float get_minimum () const { return min_value; }
	float get_maximum () const { return max_value; }
	float get_default () const { return default_value; }
	float generate_random_value () const;
	
  private:
	
	Type  type;
	float min_value;
	float max_value;
	float default_value;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_PARAMETER_H
