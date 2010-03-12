#ifndef REVERB_TUNER_UTILS_H
#define REVERB_TUNER_UTILS_H

#include <cmath>
#include <iterator>

#include "parameter_modifier.h"

namespace ReverbTuner {

template<typename T>
void clamp (T & val, T const & min, T const & max)
{
	val = std::max (val, min);
	val = std::min (val, max);
}

template<typename Container>
typename Container::iterator random_from_container (Container & container, unsigned random_uint)
{
	random_uint = random_uint % container.size ();
	typename Container::iterator it = container.begin ();
	std::advance (it, random_uint);
	return it;
}

template<typename Container>
typename Container::const_iterator random_from_container (Container const & container, unsigned random_uint)
{
	return random_from_container (const_cast<Container &> (container), random_uint);
}

} // namespace ReverbTuner

#endif // REVERB_TUNER_UTILS_H
