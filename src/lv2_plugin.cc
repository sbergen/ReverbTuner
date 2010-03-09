#include "reverbtuner/lv2_plugin.h"

namespace ReverbTuner {

Lv2Plugin::Lv2Plugin (SLV2Plugin plugin, double samplerate)
  : plugin (plugin)
  , samplerate (samplerate)
  , param_vals (param_set)
{
	instance = slv2_plugin_instantiate (plugin, samplerate, NULL);
	slv2_instance_activate (instance);
	
	// TODO parameters
}

Lv2Plugin::~Lv2Plugin()
{
	slv2_instance_free (instance);
}


Plugin *
Lv2Plugin::clone() const
{
	return new Lv2Plugin (plugin, samplerate);
}

void
Lv2Plugin::run (float * in, float * out, unsigned frames)
{
	slv2_instance_connect_port (instance, in_port_index, in);
	slv2_instance_connect_port (instance, out_port_index, out);
	slv2_instance_run (instance, frames);
	
}

void
Lv2Plugin::apply_parameters (ParameterValues const & params)
{
	param_vals = params;
}

ParameterSet const &
Lv2Plugin::get_parameters () const
{
	return param_set;
}


} // namespace ReverbTuner
