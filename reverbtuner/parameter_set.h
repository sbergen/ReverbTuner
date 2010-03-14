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
	
	// May only be called with valid indexes!
	Parameter & operator[] (unsigned index) { return *parameters.find (index)->second; } 
	Parameter const & operator[] (unsigned index) const { return *parameters.find (index)->second; }
	
	/// Takes ownership of \a parameter
	Parameter & add_parameter (unsigned index, Parameter * parameter)
		{ parameters.insert (index, parameter); return *parameter; }
	
	unsigned index_of_parameter (Parameter const * parameter) const
	{
		for (iterator it = parameters.begin(); it != parameters.end(); ++it) {
			if (it->second == parameter) { return it->first; }
		}
		throw std::invalid_argument ("ParameterSet does not have given parameter");
	}
	
	unsigned random_index (RandomGenerator & rg) const
	{
		return random_from_container (parameters, rg)->first;
	}
	
  private:
	Container parameters;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_PARAMETER_SET_H
