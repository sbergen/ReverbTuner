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
	
	Container::const_iterator begin () const { return parameters.begin(); }
	Container::const_iterator end () const { return parameters.end(); }
	Container::const_iterator find (unsigned i) const { return parameters.find (i); }
	
	/// Takes ownership of \a parameter
	void add_parameter (unsigned index, Parameter * parameter)
		{ parameters.insert (index, parameter); }
	
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
