#ifndef REVERB_TUNER_PARAMETER_MODIFIER_H
#define REVERB_TUNER_PARAMETER_MODIFIER_H

#include <boost/random/mersenne_twister.hpp>

namespace ReverbTuner {

class Parameter;

class ParameterModifier
{
  public:
	ParameterModifier (double samplerate);
	
	void randomize_uniform (float & value, Parameter const & param);
	void randomize_triangular (float & value, Parameter const & param);
	
	void make_valid (float & value, Parameter const & param);
	
  private:
	double samplerate;
	boost::mt19937 rng;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_PARAMETER_MODIFIER_H
