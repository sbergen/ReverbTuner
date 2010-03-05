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
	
	Parameter (unsigned index, float default_value, float min_value, float max_value);
	
	void set_value (float val);
	float generate_random_value ();
	
	unsigned get_index () const { return index; }
	float get_value () const { return value; }
	Type get_type () const { return type; }
	
  private:
	  
	unsigned index;
	
	Type  type;
	float value;
	float min_value;
	float max_value;
	float default_value;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_PARAMETER_H
