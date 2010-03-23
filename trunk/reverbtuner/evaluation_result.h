#ifndef REVERB_TUNER_EVALUATION_RESULT_H
#define REVERB_TUNER_EVALUATION_RESULT_H

#include <boost/operators.hpp>

namespace ReverbTuner {

/// Currently the result is only a floating point value
struct EvaluationResult
  : public boost::less_than_comparable<EvaluationResult>
  , public boost::equivalent<EvaluationResult>
  , public boost::equality_comparable<EvaluationResult>
{
	EvaluationResult () : value (0.0) {}
	EvaluationResult (float value) : value (value) {}
	
	operator float () const { return value; }
	bool operator < (EvaluationResult const & other) { return value < other.value; }

	float value;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_EVALUATION_RESULT_H
