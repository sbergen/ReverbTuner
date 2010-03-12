#ifndef REVERB_TUNER_RANDOM_GENERATOR_H
#define REVERB_TUNER_RANDOM_GENERATOR_H

#include <ctime>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_smallint.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/uniform_01.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/triangle_distribution.hpp>

namespace ReverbTuner {

class RandomGenerator
{
  public:
	
	RandomGenerator ()
	  : rng (std::time (0))
	  , uniform_generator (rng, uniform_dist)
	{}
	
	float random_uniform (float min, float max)
		{ return boost::uniform_real<float> (min, max) (rng); }
	float random_triangular (float min, float med, float max)
		{ return boost::triangle_distribution<float> (min, med, max) (uniform_generator); }
	unsigned random_less_than (unsigned max)
		{ return boost::uniform_smallint<unsigned> (0, max) (rng); }
	
	bool random_bool () { return uniform_generator() < 0.5; }
	unsigned random_uint () { return rng(); }
	
  private:
	boost::mt19937 rng;
	boost::uniform_01<> uniform_dist;
	boost::variate_generator<boost::mt19937, boost::uniform_01<> > uniform_generator;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_RANDOM_GENERATOR_H
