#include "reverbtuner/lv2_plugin_factory.h"
#include "reverbtuner/lv2_plugin.h"

#include <iostream>

using namespace ReverbTuner;

int main ()
{
	std::cout << "Loading plugins..." << std::endl;
	
	Lv2PluginFactory factory (44100);
	
	unsigned count = factory.plugin_count ();
	
	std::cout << count << " Plugins found:" << std::endl;
	
	for (unsigned i = 0; i < count; ++i) {
		std::cout << i << ". " << factory.plugin_name (i) << std::endl;
	}
	
	unsigned which;
	do {
		std::cout << "Which plugin do you want to load?" << std::endl;
		std::cin >> which;
	} while (which > count);
	
	Lv2Plugin * plugin = factory.plugin (which);
	
	delete plugin;
}
