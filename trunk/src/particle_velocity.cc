#include "reverbtuner/particle_velocity.h"

#include <cassert>

#include "reverbtuner/parameter_values.h"

namespace ReverbTuner {

ParticleVelocity::ParticleVelocity (ParameterValues & from, ParameterValues & to)
  : boost::numeric::ublas::vector<float> (from.size ())
{
	assert (from.size () == to.size ());
	
	ParameterValues::iterator from_it = from.begin ();
	ParameterValues::iterator to_it = to.begin ();
	iterator this_it = begin ();
	while (from_it != from.end ()) {
		
		*this_it = to_it->second - from_it->second;
		
		++from_it;
		++to_it;
		++this_it;
	}
}

ParameterValues &
operator+= (ParameterValues & values, ParticleVelocity const & velocity)
{
	assert (values.size () == velocity.size ());
	
	ParameterValues::iterator values_it = values.begin ();
	ParticleVelocity::const_iterator velocity_it = velocity.begin ();
	
	while (values_it != values.end ()) {
		values_it->second += *velocity_it;
		
		++values_it;
		++velocity_it;
	}
	
	return values;
}

std::ostream & operator<< (std::ostream & stream, ParticleVelocity const & velocity)
{
	stream << "[ ";
	for (ParticleVelocity::const_iterator it = velocity.begin (); it != velocity.end (); ++it) {
		stream << *it << " ";
	}
	stream << "]";
	return stream;
}

} // namespace ReverbTuner
