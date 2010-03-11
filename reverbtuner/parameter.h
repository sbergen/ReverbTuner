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
	  : _type (type)
	  , _min_value (min_value)
	  , _max_value (max_value)
	  , _default_value (default_value)
	{}
	
	Type type () const { return _type; }
	float min () const { return _min_value; }
	float max () const { return _max_value; }
	float def () const { return _default_value; }
	
  private:
	
	Type  _type;
	float _min_value;
	float _max_value;
	float _default_value;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_PARAMETER_H
