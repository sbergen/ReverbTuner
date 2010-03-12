#ifndef REVERB_TUNER_PARAMETER_MODIFIER_H
#define REVERB_TUNER_PARAMETER_MODIFIER_H

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/uniform_01.hpp>

namespace ReverbTuner {

class Parameter;
class ParameterSet;

class ParameterModifier
{
  public:
	ParameterModifier (double samplerate);
	
	void randomize_uniform (float & value, Parameter const & param);
	void randomize_triangular (float & value, Parameter const & param);
	
	void make_valid (float & value, Parameter const & param);
	
	bool random_bool () { return uniform_generator() < 0.5; }
	unsigned random_uint () { return rng(); }
	unsigned random_with_max_value (unsigned max);
	
	/// Get a random (existing) index from set
	unsigned random_index (ParameterSet const & set);
	
  private:
	double samplerate;
	
	boost::mt19937 rng;
	boost::uniform_01<> uniform_dist;
	boost::variate_generator<boost::mt19937, boost::uniform_01<> > uniform_generator;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_PARAMETER_MODIFIER_H
