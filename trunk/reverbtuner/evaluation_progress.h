#ifndef REVERB_TUNER_EVALUATION_PROGRESS_H
#define REVERB_TUNER_EVALUATION_PROGRESS_H

#include <boost/thread.hpp>

namespace ReverbTuner {

class EvaluationProgress
{
  public:
	EvaluationProgress ()
	  : _best_result (-std::numeric_limits<float>::infinity())
	  , _total_rounds (0)
	  , _current_round (0)
	  , _aborted (false)
	  , _done (false)
	{}

	void set_best_result (float result) { LockGuard lg (mutex); _best_result = result; }
	void set_current_round (unsigned round) { LockGuard lg (mutex); _current_round = round; }
	void set_total_rounds (unsigned rounds) { LockGuard lg (mutex); _total_rounds = rounds; }
	
	float best_result () const { LockGuard lg (mutex); return _best_result; }
	unsigned current_round () const { LockGuard lg (mutex); return _current_round; }
	unsigned total_rounds () const { LockGuard lg (mutex); return _total_rounds; }
	
	float progress_percentage () const
	{
		LockGuard lg (mutex);
		float ret = static_cast<float> (_current_round) / _total_rounds;
		return ret;
	}
	
	void abort () { LockGuard lg (mutex); _aborted = true; }
	bool aborted () const { LockGuard lg (mutex); return _aborted; }
	
	void set_done () { LockGuard lg (mutex); _done = true; }
	bool done () const { LockGuard lg (mutex); return _done; }

  private:
	mutable boost::mutex mutex;
	typedef boost::lock_guard<boost::mutex> LockGuard;
	
	float _best_result;
	unsigned _total_rounds;
	unsigned _current_round;
	
	bool _aborted;
	bool _done;

};

} // namespace ReverbTuner

#endif // REVERB_TUNER_EVALUATION_PROGRESS_H
