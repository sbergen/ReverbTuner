#ifndef REVERB_TUNER_PARAMETER_MODIFIER_H
#define REVERB_TUNER_PARAMETER_MODIFIER_H

namespace ReverbTuner {

class Parameter;
class ParameterSet;
class ParameterValues;
class RandomGenerator;

class ParameterModifier
{
  public:
	ParameterModifier (double samplerate, RandomGenerator & rg);
	
	void randomize_uniform (float & value, Parameter const & param);
	void randomize_triangular (float & value, Parameter const & param);
	
	/** Randomize all parameters
	 * @param uniform_probability probablility for using a uniform distribution instead of triangular
	 **/
	void randomize_all (ParameterValues & values, float uniform_probability);
	
  private:
	double samplerate;
	RandomGenerator & rg;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_PARAMETER_MODIFIER_H
