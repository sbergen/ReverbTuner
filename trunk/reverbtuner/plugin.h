#ifndef REVERB_TUNER_PLUGIN_H
#define REVERB_TUNER_PLUGIN_H

#include <boost/shared_ptr.hpp>

#include "reverbtuner/types.h"

namespace ReverbTuner {

class ParameterValues;
class ParameterSet;

class Plugin
{
  public:

	virtual ~Plugin() {}
	
	virtual SharedPluginPtr clone() const = 0;
	virtual void reset () = 0;
	virtual void run (float const * in, float * out, unsigned frames) = 0;
	virtual unsigned latency () = 0;
	virtual void apply_parameters (ParameterValues const & parameters) = 0;
	virtual ParameterSet const & get_parameters () const = 0;

};

} // namespace ReverbTuner

#endif // REVERB_TUNER_PLUGIN_H
