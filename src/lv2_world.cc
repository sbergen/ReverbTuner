#include "reverbtuner/lv2_world.h"

namespace ReverbTuner {

Lv2World::Lv2World (double samplerate)
  : samplerate (samplerate)
{
	world = slv2_world_new ();
	slv2_world_load_all (world);
	plugins = slv2_world_get_all_plugins (world);
	
	// Setup URIs
	input_class = slv2_value_new_uri (world, SLV2_PORT_CLASS_INPUT);
	output_class = slv2_value_new_uri (world, SLV2_PORT_CLASS_OUTPUT);
	control_class = slv2_value_new_uri (world, SLV2_PORT_CLASS_CONTROL);
	audio_class = slv2_value_new_uri (world, SLV2_PORT_CLASS_AUDIO);
	integer = slv2_value_new_uri(world, SLV2_NAMESPACE_LV2 "integer");
	toggled = slv2_value_new_uri(world, SLV2_NAMESPACE_LV2 "toggled");
	srate = slv2_value_new_uri(world, SLV2_NAMESPACE_LV2 "sampleRate");
}

Lv2World::~Lv2World ()
{
	slv2_value_free (input_class);
	slv2_value_free (output_class);
	slv2_value_free (control_class);
	slv2_value_free (audio_class);
	slv2_value_free (integer);
	slv2_value_free (toggled);
	slv2_value_free (srate);
	
	
	slv2_plugins_free (world, plugins);
	slv2_world_free (world);
}

float
Lv2World::value_as_float (SLV2Value val)
{
	float ret = val ? slv2_value_as_float (val) : 0.0;
	slv2_value_free(val);
	return ret;
}

std::string
Lv2World::value_as_string (SLV2Value val)
{
	std::string str (val ? slv2_value_as_string (val) : "");
	slv2_value_free (val);
	return str;
}

} // namespace ReverbTuner
