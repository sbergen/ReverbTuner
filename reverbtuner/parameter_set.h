#ifndef REVERB_TUNER_PARAMETER_SET_H
#define REVERB_TUNER_PARAMETER_SET_H

#include <stdexcept>

#include <boost/ptr_container/ptr_map.hpp>

#include "parameter.h"
#include "utils.h"

namespace ReverbTuner {

class Parameter;

class ParameterSet
{
  public:
	
	typedef boost::ptr_map<unsigned, Parameter> Container;
	typedef Container::const_iterator iterator;
	
	iterator begin () const { return parameters.begin(); }
	iterator end () const { return parameters.end(); }
	iterator find (unsigned i) const { return parameters.find (i); }
	
	/// Takes ownership of \a parameter
	void add_parameter (unsigned index, Parameter * parameter)
		{ parameters.insert (index, parameter); }
	
	unsigned index_of_parameter (Parameter const * parameter) const
	{
		for (iterator it = parameters.begin(); it != parameters.end(); ++it) {
			if (it->second == parameter) { return it->first; }
		}
		throw std::invalid_argument ("ParameterSet does not have given parameter");
	}
	
	unsigned index_from_random_number (unsigned random) const
	{
		return random_from_container (parameters, random)->first;
	}
	
  private:
	Container parameters;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_PARAMETER_SET_H
