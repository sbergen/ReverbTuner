#ifndef REVERB_TUNER_EVALUATION_RESOURCES_H
#define REVERB_TUNER_EVALUATION_RESOURCES_H

namespace ReverbTuner {

/// Base class for storing (memory) resources needed during parameter evaluation
class EvaluationResources
{
  public:
	EvaluationResources ();
	virtual ~EvaluationResources ();
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_EVALUATION_RESOURCES_H
