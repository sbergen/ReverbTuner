#ifndef REVERB_TUNER_PARAMETER_SET_H
#define REVERB_TUNER_PARAMETER_SET_H

#include <boost/ptr_container/ptr_vector.hpp>
#include <vector>

namespace ReverbTuner {

class Parameter;

class ParameterSet
{
  public:
	
	ParameterSet ();
	~ParameterSet ();
	
	ParameterSet & operator= (ParameterSet const & other);
	
	typedef boost::ptr_vector<Parameter> Container;
	
	Container const & data() const { return parameters; }
	Container & data() { return parameters; }
	
  private:
	Container parameters;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_PARAMETER_SET_H
