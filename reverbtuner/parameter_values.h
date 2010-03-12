#ifndef REVERB_TUNER_PARAMETER_VALUES_H
#define REVERB_TUNER_PARAMETER_VALUES_H

#include <map>

namespace ReverbTuner {

class ParameterSet;
class Parameter;

class ParameterValues
{
  private:
	typedef std::map<unsigned, float> Values;
	
  public:
	
	ParameterValues (ParameterSet const & set);
	ParameterValues (ParameterValues const & other);
	ParameterValues & operator= (ParameterValues const & other);
	
	typedef Values::iterator iterator;
	iterator begin() { return values.begin(); }
	iterator end() { return values.end(); }
	
	ParameterSet const & get_set () { return set; }
	
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
