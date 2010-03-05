#ifndef REVERB_TUNER_LV2_PLUGIN_H
#define REVERB_TUNER_LV2_PLUGIN_H

#include <slv2.h>

#include "parameter_set.h"

namespace ReverbTuner {

class Lv2Plugin
{
  public:

	Lv2Plugin (SLV2Plugin plugin, double samplerate);
	~Lv2Plugin();
	
	Plugin * clone();
	void run (float * in, float * out, unsigned frames);
	void apply_parameters (ParameterSet const & params);
	void get_parameters (ParameterSet & params);
	
  private:
	SLV2Plugin plugin;
	double samplerate;
	SLV2Instance istance;
	
	unsigned in_index;
	unsigned out_index;
	
	ParameterSet parameters;

};

} // namespace ReverbTuner

#endif // REVERB_TUNER_LV2_PLUGIN_H
