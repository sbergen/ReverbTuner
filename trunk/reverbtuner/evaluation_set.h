#ifndef REVERB_TUNER_EVALUATION_SET_H
#define REVERB_TUNER_EVALUATION_SET_H

#include "reverbtuner/types.h"

namespace ReverbTuner {


class EvaluationSet
{
  public:
	
	typedef boost::function<void (ParameterValues &)> ParameterInitializer;
	
	virtual void go_to_first () const = 0;
	virtual bool next_pair (ParameterValues *& parameters, EvaluationResult *& result) throw() = 0;
	virtual bool next_pair (ParameterValues const *& parameters, EvaluationResult *& result) const throw()
		{ return const_cast<EvaluationSet &> (*this).next_pair (const_cast<ParameterValues *&> (parameters), result); }
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_EVALUATION_SET_H
