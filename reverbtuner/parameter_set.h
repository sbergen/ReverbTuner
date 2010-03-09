#ifndef REVERB_TUNER_PARAMETER_SET_H
#define REVERB_TUNER_PARAMETER_SET_H

#include <stdexcept>
#include <boost/ptr_container/ptr_map.hpp>
#include "parameter.h"

namespace ReverbTuner {

class Parameter;

class ParameterSet
{
  public:
	
	typedef boost::ptr_map<unsigned, Parameter> Container;
	
	Container const & data() const { return parameters; }
	Container & data() { return parameters; }
	
	unsigned index_of_parameter (Parameter const * parameter) const
	{
		for (Container::const_iterator it = parameters.begin(); it != parameters.end(); ++it) {
			if (it->second == parameter) { return it->first; }
		}
		throw std::invalid_argument ("ParameterSet does not have given parameter");
	}
	
  private:
	Container parameters;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_PARAMETER_SET_H
