#ifndef REVERB_TUNER_LV2_PLUGIN_H
#define REVERB_TUNER_LV2_PLUGIN_H

#include <slv2/slv2.h>

#include "plugin.h"
#include "parameter_set.h"
#include "parameter_values.h"

namespace ReverbTuner {

class Lv2World;

class Lv2Plugin : public Plugin
{
  public:

	Lv2Plugin (Lv2World & world, SLV2Plugin plugin);
	~Lv2Plugin();
	
	Plugin * clone() const;
	void reset ();
	void run (float const * in, float * out, unsigned frames);
	void apply_parameters (ParameterValues const & params);
	ParameterSet const & get_parameters () const;
	
  private:
	
	void init_params_from_plugin ();
	void add_parameter_from_port (unsigned index, SLV2Port port);
	float value_as_float (SLV2Value val);
	
	Lv2World & world;
	SLV2Plugin plugin;
	SLV2Instance instance;
	
	unsigned in_port_index;
	unsigned out_port_index;
	
	ParameterSet    param_set;
	ParameterValues param_vals;

};

} // namespace ReverbTuner

#endif // REVERB_TUNER_LV2_PLUGIN_H
