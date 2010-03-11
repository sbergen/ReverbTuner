#ifndef REVERB_TUNER_EVALUATION_RESULT_H
#define REVERB_TUNER_EVALUATION_RESULT_H

namespace ReverbTuner {

/// Currently the result is only a floating point value
struct EvaluationResult
{
	EvaluationResult () : value (0.0) {}
	EvaluationResult (float value) : value (value) {}
	// Comparison etc.
	operator float() { return value; }

	float value;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_EVALUATION_RESULT_H
