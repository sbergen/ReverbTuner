#ifndef REVERB_TUNER_PARAMETER_VALUES_H
#define REVERB_TUNER_PARAMETER_VALUES_H

#include <map>
#include <iostream>

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
	void apply_validated_values (ParameterValues const & other, double samplerate);
	
	unsigned size () const { return values.size (); }
	
	typedef Values::iterator iterator;
	typedef Values::const_iterator const_iterator;
	
	iterator begin() { return values.begin(); }
	iterator end() { return values.end(); }
	
	const_iterator begin() const { return values.begin(); }
	const_iterator end() const { return values.end(); }
	
	ParameterSet const & get_set () const { return set; }
	
	float & operator[] (unsigned index);
	float operator[] (unsigned index) const;
	
	float & operator[] (Parameter const & param);
	float operator[] (Parameter const & param) const;
	
  private:
	
	float validate_value (float value, Parameter const & param, double samplerate);
	
	ParameterSet const & set;
	Values values;
};

std::ostream & operator<< (std::ostream & stream, ParameterValues const & values);

} // namespace ReverbTuner

#endif // REVERB_TUNER_PARAMETER_VALUES_H
