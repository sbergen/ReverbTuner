#ifndef REVERB_TUNER_LV2_LV2_WORLD_H
#define REVERB_TUNER_LV2_LV2_WORLD_H

#include <slv2/slv2.h>

namespace ReverbTuner {

struct Lv2World
{
	Lv2World (double samplerate);
	~Lv2World ();
	
	double      samplerate;
	SLV2World   world;
	SLV2Plugins plugins;
	
	SLV2Value input_class;
	SLV2Value output_class;
	SLV2Value control_class;
	SLV2Value audio_class;
	SLV2Value integer;
	SLV2Value toggled;
	SLV2Value srate;

};

} // namespace ReverbTuner

#endif // REVERB_TUNER_LV2_LV2_WORLD_H


