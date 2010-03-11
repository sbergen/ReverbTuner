#ifndef REVERB_TUNER_UTILS_H
#define REVERB_TUNER_UTILS_H

#include <cmath>

namespace ReverbTuner {

template<typename T>
void clamp (T & val, T const & min, T const & max)
{
	val = std::max (val, min);
	val = std::min (val, max);
}

} // namespace ReverbTuner

#endif // REVERB_TUNER_UTILS_H
