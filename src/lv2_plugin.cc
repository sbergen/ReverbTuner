#include "reverbtuner/lv2_plugin.h"
#include "reverbtuner/lv2_world.h"

#include <stdexcept>
#include <iostream>

namespace ReverbTuner {

Lv2Plugin::Lv2Plugin (Lv2World & world, SLV2Plugin plugin, double samplerate)
  : world (world)
  , plugin (plugin)
  , samplerate (samplerate)
  , param_vals (param_set)
{
	instance = slv2_plugin_instantiate (plugin, samplerate, NULL);
	init_params_from_plugin ();
	slv2_instance_activate (instance);
}

Lv2Plugin::~Lv2Plugin()
{
	slv2_instance_free (instance);
}


SharedPluginPtr
Lv2Plugin::clone() const
{
	return SharedPluginPtr (new Lv2Plugin (world, plugin, samplerate));
}

void
Lv2Plugin::reset ()
{
	// Update latency port by running for one sample with current settings
	// Some plugins crash when run for 0 frames
	float dummy = 0.0;
	run (&dummy, &dummy, 1);
	
	slv2_instance_deactivate (instance);
	slv2_instance_activate (instance);
}

void
Lv2Plugin::run (float const * in, float * out, unsigned frames)
{
	ensure_dummy_buffers (frames);
	
	// In port const cast should be safe
	slv2_instance_connect_port (instance, in_port_index, const_cast<float *> (in));
	slv2_instance_connect_port (instance, out_port_index, out);
	slv2_instance_run (instance, frames);
}

unsigned
Lv2Plugin::latency ()
{
	return current_latency > 0 ? current_latency : 0;
}

void
Lv2Plugin::apply_parameters (ParameterValues const & params)
{
	param_vals.apply_validated_values (params, samplerate);
}

ParameterSet const &
Lv2Plugin::get_parameters () const
{
	return param_set;
}

void
Lv2Plugin::init_params_from_plugin ()
{
	dummy_in_indices.clear ();
	dummy_out_indices.clear ();
	
	in_port_index = -1;
	out_port_index = -1;
	current_latency = 0;
	
	unsigned num_ports = slv2_plugin_get_num_ports (plugin);
	for (unsigned i = 0; i < num_ports; i++) {
		SLV2Port port = slv2_plugin_get_port_by_index (plugin, i);
		
		if (slv2_port_is_a (plugin, port, world.input_class)) {
			add_input_from_port (i, port);
		} else if (slv2_port_is_a (plugin, port, world.output_class)) {
			add_output_from_port (i, port);
		}
	}
	
	if (out_port_index == -1 || in_port_index == -1) {
		throw std::logic_error ("Could not find appropriate ports from plugin");
	}
}

void
Lv2Plugin::add_input_from_port (unsigned index, SLV2Port port)
{
	// Handle audio port as possible input
	if (slv2_port_is_a (plugin, port, world.audio_class) && in_port_index == -1) {
			in_port_index = index;
	// then control ports as parameters
	} else if (slv2_port_is_a (plugin, port, world.control_class)) {
		add_parameter_from_port (index, port);
	// optional to NULL
	} else if (slv2_port_has_property (plugin, port, world.optional)) {
		slv2_instance_connect_port (instance, index, NULL);
	// else dummy
	} else {
		dummy_in_indices.push_back (index);
	}
}

void
Lv2Plugin::add_output_from_port (unsigned index, SLV2Port port)
{
	// Handle audio ports as possible output
	if (slv2_port_is_a (plugin, port, world.audio_class) && out_port_index == -1) {
		out_port_index = index;
	// then latency port
	} else if (slv2_port_has_property (plugin, port, world.latency)) {
		slv2_instance_connect_port (instance, index, &current_latency);
	// optional to NULL
	} else if (slv2_port_has_property (plugin, port, world.optional)) {
		slv2_instance_connect_port (instance, index, NULL);
	// else dummy
	} else {
		dummy_out_indices.push_back (index);
	}
		 
}

void
Lv2Plugin::add_parameter_from_port (unsigned index, SLV2Port port)
{
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
	float def_value = Lv2World::value_as_float (def);
	
	// Generate parameter
	Parameter & parameter = param_set.add_parameter (
		index, new Parameter (
			type, def_value,
			Lv2World::value_as_float (min),
			Lv2World::value_as_float (max)
		));
	parameter.set_name (Lv2World::value_as_string (slv2_port_get_name (plugin, port)));
	
	param_vals[index] = def_value;
	slv2_instance_connect_port (instance, index, &param_vals[index]);
}

void
Lv2Plugin::ensure_dummy_buffers (unsigned buffer_size)
{
	if (dummy_in_buffer.size() >= buffer_size) { return; }
	
	dummy_in_buffer.resize (buffer_size);
	dummy_out_buffer.resize (buffer_size);
	
	for (std::vector<unsigned>::const_iterator it = dummy_in_indices.begin(); it != dummy_in_indices.end(); ++it) {
		slv2_instance_connect_port (instance, *it, &dummy_in_buffer[0]);
	}
	
	for (std::vector<unsigned>::const_iterator it = dummy_out_indices.begin(); it != dummy_out_indices.end(); ++it) {
		slv2_instance_connect_port (instance, *it, &dummy_out_buffer[0]);
	}
}

} // namespace ReverbTuner
