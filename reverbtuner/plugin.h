#ifndef REVERB_TUNER_PLUGIN_H
#define REVERB_TUNER_PLUGIN_H

namespace ReverbTuner {

class ParameterSet;

class Plugin
{
  public:

	Plugin();
	virtual ~Plugin();
	
	virtual Plugin * clone() = 0;
	virtual void run (float * in, float * out, unsigned frames) = 0;
	virtual void apply_parameters (ParameterSet const & parameters) = 0;

  private:
	
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_PLUGIN_H
