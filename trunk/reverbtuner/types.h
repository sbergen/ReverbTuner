#ifndef REVERB_TUNER_TYPES_H
#define REVERB_TUNER_TYPES_H

#include <map>

/*************** Forward declarations ***********************/

namespace boost {
	class mutex;
	
	template<typename T> class shared_ptr;
	template<typename T> class scoped_ptr;
	template<typename T> class lock_guard;
} // namespace boost

namespace ReverbTuner {

class DataSource;
class EvaluationProgress;
class EvaluationResult;
class EvaluationScheduler;
class EvaluationSet;
class Evaluator;
class EvolutionaryOptimizer;
class Lv2PluginFactory;
class Lv2Plugin;
class Lv2World;
class MfccEvaluator;
class MfccProcessor;
class Parameter;
class ParameterModifier;
class ParameterSet;
class ParameterValues;
class PluginFactory;
class Plugin;
class RandomGenerator;
class Runner;
class SwarmEvaluationSet;

template<typename EvaluatorType> class SingleThreadedScheduler;
template<typename EvaluatorType> class ThreadedScheduler;

/************************* Typedefs ***********************/

typedef boost::shared_ptr<DataSource> SharedDataSourcePtr;
typedef boost::shared_ptr<EvaluationProgress> SharedEvaluationProgressPtr;
typedef boost::shared_ptr<Plugin> SharedPluginPtr;
typedef boost::shared_ptr<Runner> SharedRunnerPtr;

typedef boost::scoped_ptr<ParameterValues> ScopedParameterValuesPtr;

typedef boost::lock_guard<boost::mutex> LockGuard;

} // namespace ReverbTuner

#endif // REVERB_TUNER_TYPES_H
