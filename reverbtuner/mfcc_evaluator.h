#ifndef REVERB_TUNER_MFCC_EVALUATOR_H
#define REVERB_TUNER_MFCC_EVALUATOR_H

#include <vector>

#include <boost/shared_ptr.hpp>

#include "reverbtuner/evaluator.h"
#include "reverbtuner/mfcc_processor.h"

namespace ReverbTuner {

class Plugin;

/// mel frequency cepstrum coefficient evaluator
class MfccEvaluator : public Evaluator
{
  public:
	MfccEvaluator (DataSource const & data_source);
	~MfccEvaluator ();

	void evaluate_parameters (ParameterValues const & parameters, EvaluationResult & result);

  private:
	
	typedef std::vector<float> Data;
	typedef std::vector<Data> CoefData;
	
	void init_static_data (DataSource const & data_source);
	void init_coef_data (CoefData & data);
	void run_mfcc (Data const & in, CoefData & result, unsigned frames, bool run_plugin);
	float coef_difference (CoefData const & a, CoefData const & b);
	float euclidean_distance (Data const & a, Data const & b);
	
	boost::shared_ptr<Plugin> plugin;
	MfccProcessor processor;
	
	Data analysis_buffer;
	CoefData result_coefs;
	
  private:
	
	// Static const data for now, could make these adjustable...
	static const int mfcc_coefs;
	static const int mfcc_buffer_size;
	static const int mfcc_hop_size;
	
	// This relies on the fact that there is only one evaluation running per process
	static bool static_init_done;
	static unsigned target_length;
	static CoefData target_coefs;
	static float target_zero_difference;
	static const Data zero_buffer;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_MFCC_EVALUATOR_H
