#ifndef REVERB_TUNER_PARAMETER_VALUES_H
#define REVERB_TUNER_PARAMETER_VALUES_H

#include <map>

namespace ReverbTuner {

class ParameterSet;
class Parameter;

class ParameterValues
{
  public:
	
	ParameterValues (ParameterSet const & set);
	
	ParameterValues & operator= (ParameterValues const & other);
	
	typedef std::map<unsigned, float> Values;
	
	float & operator[] (unsigned index);
	float operator[] (unsigned index) const;
	
	float & operator[] (Parameter const & param);
	float operator[] (Parameter const & param) const;
	
  private:
	ParameterSet const & set;
	Values values;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_PARAMETER_VALUES_H
