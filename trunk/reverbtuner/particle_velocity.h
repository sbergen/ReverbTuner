#ifndef REVERB_TUNER_PARTICLE_VELOCITY_H
#define REVERB_TUNER_PARTICLE_VELOCITY_H

#include <iostream>

#include <boost/numeric/ublas/vector.hpp>

#include "reverbtuner/types.h"

namespace ReverbTuner {

class ParticleVelocity : public boost::numeric::ublas::vector<float>
{
  public:
	// Default constructor allowed
	ParticleVelocity () {}
	
	/// Construct a velocity from \a from to \a to
	ParticleVelocity (ParameterValues & from, ParameterValues & to);
	
	/// Allow access to base class operator*=
	template<typename T>
	ParticleVelocity & operator*= (T const & other)
	{
		static_cast<boost::numeric::ublas::vector<float> &> (*this) *= other; return *this;
	}
	
	/// Override operator*= for ParticleVelocity
	ParticleVelocity & operator*= (ParticleVelocity const & other)
	{
		static_cast<boost::numeric::ublas::vector<float> &> (*this) = 
			boost::numeric::ublas::element_prod (*this, other);
		return *this;
	}
	
	float norm () const
		{ return boost::numeric::ublas::norm_2 (*this); }
};

ParameterValues & operator+= (ParameterValues & values, ParticleVelocity const & velocity);

std::ostream & operator<< (std::ostream & stream, ParticleVelocity const & velocity);

} // namespace ReverbTuner

#endif // REVERB_TUNER_PARTICLE_VELOCITY_H
