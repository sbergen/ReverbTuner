#include "reverbtuner/lv2_plugin.h"
#include "reverbtuner/lv2_world.h"

#include <stdexcept>
#include <iostream>

namespace ReverbTuner {

Lv2Plugin::Lv2Plugin (Lv2World & world, SLV2Plugin plugin)
  : world (world)
  , plugin (plugin)
  , param_vals (param_set)
{
	instance = slv2_plugin_instantiate (plugin, world.samplerate, NULL);
	init_params_from_plugin ();
	slv2_instance_activate (instance);
}

Lv2Plugin::~Lv2Plugin()
{
	slv2_instance_free (instance);
}


PluginPtr
Lv2Plugin::clone() const
{
	return PluginPtr (new Lv2Plugin (world, plugin));
}

void
Lv2Plugin::reset ()
{
	slv2_instance_deactivate (instance);
	slv2_instance_activate (instance);
}

void
Lv2Plugin::run (float const * in, float * out, unsigned frames)
{
	ensure_dummy_buffer (frames);
	
	// In port const cast should be safe
	slv2_instance_connect_port (instance, in_port_index, const_cast<float *> (in));
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

void
Lv2Plugin::init_params_from_plugin ()
{
	dummy_indices.clear ();
	bool input_found = false;
	bool output_found = false;
	unsigned num_ports = slv2_plugin_get_num_ports (plugin);
	
	for (unsigned i = 0; i < num_ports; i++) {
		SLV2Port port = slv2_plugin_get_port_by_index (plugin, i);
		
		if (slv2_port_is_a (plugin, port, world.input_class)) {
			if (slv2_port_is_a (plugin, port, world.audio_class)) {
				if (!input_found) {
					in_port_index = i;
					input_found = true;
				} else {
					dummy_indices.push_back (i);
				}
			} else if (slv2_port_is_a (plugin, port, world.control_class)) {
				add_parameter_from_port (i, port);
			}
		} else if (slv2_port_is_a (plugin, port, world.output_class)) {
			if (slv2_port_is_a (plugin, port, world.audio_class)) {
				if (!output_found) {
					out_port_index = i;
					output_found = true;
				} else {
					dummy_indices.push_back (i);
				}
			}
		}
	}
	
	if (!output_found || !input_found) {
		throw std::logic_error ("Could not find appropriate ports from plugin");
	}
}

void
Lv2Plugin::add_parameter_from_port (unsigned index, SLV2Port port)
{
	enum Type {
		TypeNormal,
		TypeInteger,
		TypeSampleRate,
		TypeToggled
	};
	
	Parameter::Type type = Parameter::TypeNormal;
	
	if (slv2_port_has_property (plugin, port, world.integer)) {
		type = Parameter::TypeInteger;
	} else if (slv2_port_has_property (plugin, port, world.toggled)) {
		type = Parameter::TypeToggled;
	} else if (slv2_port_has_property (plugin, port, world.srate)) {
		type = Parameter::TypeSampleRate;
	}
	
	// Default min and max
	SLV2Value def, min, max;
	slv2_port_get_range (plugin, port, &def, &min, &max);
	float def_value = value_as_float (def);
	
	// Generate parameter
	ParameterSet::Container & params = param_set.data();
	params.insert (index,
		new Parameter (
			type,
			def_value,
			value_as_float (min),
			value_as_float (max)
		));
	
	param_vals[index] = def_value;
	slv2_instance_connect_port (instance, index, &param_vals[index]);
}

float
Lv2Plugin::value_as_float (SLV2Value val)
{
	float ret = val ? slv2_value_as_float (val) : 0.0;
	slv2_value_free(val);
	return ret;
}

void
Lv2Plugin::ensure_dummy_buffer (unsigned buffer_size)
{
	if (dummy_buffer.size() >= buffer_size) { return; }
	
	dummy_buffer.resize (buffer_size);
	for (std::vector<unsigned>::const_iterator it = dummy_indices.begin(); it != dummy_indices.end(); ++it) {
		slv2_instance_connect_port (instance, *it, &dummy_buffer[0]);
	}
}

} // namespace ReverbTuner
