#ifndef REVERB_TUNER_UTILS_H
#define REVERB_TUNER_UTILS_H

#include <cmath>
#include <iterator>

#include "parameter_modifier.h"
#include "random_generator.h"

namespace ReverbTuner {

template<typename T>
void clamp (T & val, T const & min, T const & max)
{
	val = std::max (val, min);
	val = std::min (val, max);
}

template<typename Container>
typename Container::iterator random_from_container (Container & container, RandomGenerator & rg)
{
	typename Container::size_type index = rg.random_less_than (container.size ());
	typename Container::iterator it = container.begin ();
	std::advance (it, index);
	return it;
}

template<typename Container>
typename Container::const_iterator random_from_container (Container const & container, RandomGenerator & rg)
{
	return random_from_container (const_cast<Container &> (container), rg);
}

} // namespace ReverbTuner

#endif // REVERB_TUNER_UTILS_H
