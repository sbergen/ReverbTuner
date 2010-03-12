#ifndef REVERB_TUNER_PARAMETER_MODIFIER_H
#define REVERB_TUNER_PARAMETER_MODIFIER_H

namespace ReverbTuner {

class Parameter;
class ParameterSet;
class RandomGenerator;

class ParameterModifier
{
  public:
	ParameterModifier (double samplerate, RandomGenerator & rg);
	
	void randomize_uniform (float & value, Parameter const & param);
	void randomize_triangular (float & value, Parameter const & param);
	void make_valid (float & value, Parameter const & param);	
	
  private:
	double samplerate;
	RandomGenerator & rg;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_PARAMETER_MODIFIER_H
