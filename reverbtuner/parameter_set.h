#ifndef REVERB_TUNER_PARAMETER_SET_H
#define REVERB_TUNER_PARAMETER_SET_H

#include <boost/ptr_container/ptr_map.hpp>

namespace ReverbTuner {

class Parameter;

class ParameterSet
{
  public:
	
	ParameterSet ();
	~ParameterSet ();
	
	typedef boost::ptr_map<unsigned, Parameter> Container;
	
	Container const & data() const { return parameters; }
	Container & data() { return parameters; }
	
  private:
	Container parameters;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_PARAMETER_SET_H
