#ifndef REVERB_TUNER_LV2_PLUGIN_H
#define REVERB_TUNER_LV2_PLUGIN_H

#include <slv2/slv2.h>

#include "plugin.h"
#include "parameter_set.h"
#include "parameter_values.h"

namespace ReverbTuner {

class Lv2Plugin : public Plugin
{
  public:

	Lv2Plugin (SLV2Plugin plugin, double samplerate);
	~Lv2Plugin();
	
	Plugin * clone() const;
	void run (float * in, float * out, unsigned frames);
	void apply_parameters (ParameterValues const & params);
	ParameterSet const & get_parameters () const;
	
  private:
	SLV2Plugin plugin;
	double samplerate;
	SLV2Instance instance;
	
	unsigned in_port_index;
	unsigned out_port_index;
	
	ParameterSet    param_set;
	ParameterValues param_vals;

};

} // namespace ReverbTuner

#endif // REVERB_TUNER_LV2_PLUGIN_H
