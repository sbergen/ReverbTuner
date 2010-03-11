#include "reverbtuner/mfcc_evaluator.h"

#include <algorithm>
#include <cmath>

#include "reverbtuner/data_source.h"
#include "reverbtuner/plugin.h"
#include "reverbtuner/evaluation_result.h"

namespace ReverbTuner {

const int MfccEvaluator::mfcc_coefs = 40; // Aubio currently only supports 40 bands
const int MfccEvaluator::mfcc_buffer_size = 2048;
const int MfccEvaluator::mfcc_hop_size = 1024;

bool MfccEvaluator::static_init_done = false;
unsigned MfccEvaluator::target_length = 0;
MfccEvaluator::CoefData MfccEvaluator::target_coefs;
const MfccEvaluator::Data MfccEvaluator::zero_buffer = MfccEvaluator::Data (mfcc_buffer_size, 0.0);

MfccEvaluator::MfccEvaluator (DataSource const & data_source)
  : Evaluator (data_source)
  , plugin (data_source.get_plugin()->clone())
  , processor (mfcc_buffer_size, mfcc_hop_size, mfcc_coefs, mfcc_coefs, data_source.get_samplerate())
{
	analysis_buffer.resize (mfcc_buffer_size);
	
	if (!static_init_done) {
		init_static_data (data_source);
	}
	
	init_coef_data (result_coefs);
}

MfccEvaluator::~MfccEvaluator ()
{
}

void
MfccEvaluator::evaluate_parameters (ParameterValues const & parameters, EvaluationResult & result)
{
	plugin->apply_parameters (parameters);
	DataSource::Data const & data = data_source.get_dry_data();
	run_mfcc (data, result_coefs, target_length, true);
	
	float sum = 0.0;
	CoefData::const_iterator t_it, r_it;
	for (t_it = target_coefs.begin(), r_it = result_coefs.begin(); t_it != target_coefs.end() && r_it != result_coefs.end(); ++t_it, ++r_it) {
		sum += euclidean_distance (*t_it, *r_it);
	}
	
	result = EvaluationResult (sum);
}

void
MfccEvaluator::init_static_data (DataSource const & data_source)
{
	DataSource::Data const & target_data = data_source.get_target_data();
	target_length = target_data.size();
	init_coef_data (target_coefs);
	run_mfcc (target_data, target_coefs, target_length, false);
	
	static_init_done = true;
}

void
MfccEvaluator::init_coef_data (CoefData & data)
{
	unsigned rounds = target_length / mfcc_hop_size;
	if (target_length % mfcc_hop_size != 0) {
		rounds++;
	}

	data.resize (rounds);
	for (CoefData::iterator it = data.begin(); it != data.end(); ++it) {
		it->resize (mfcc_coefs);
	}
}

void
MfccEvaluator::run_mfcc (Data const & in, CoefData & result, unsigned frames, bool run_plugin)
{
	unsigned data_frames = in.size();
	unsigned position = 0;
	unsigned round = 0;
	
	if (run_plugin) { plugin->reset(); }
	
	// First run whole frames of data (in place)
	while ((data_frames - position) > mfcc_buffer_size) {
		if (run_plugin) {
			plugin->run (&in[position], &analysis_buffer[0], mfcc_buffer_size);
			processor.run (&analysis_buffer[0], &result[round][0]);
		} else {
			processor.run (&in[position], &result[round][0]);
		}
		position += mfcc_buffer_size;
		round++;
	}
	
	// Then zero-pad last buffer that contains data
	std::fill (analysis_buffer.begin(), analysis_buffer.end(), 0.0);
	std::copy (&in[position], &in[in.size()], analysis_buffer.begin());
	if (run_plugin) {
		plugin->run (&analysis_buffer[0], &analysis_buffer[0], mfcc_buffer_size);
		processor.run (&analysis_buffer[0], &result[round][0]);
	} else {
		processor.run (&analysis_buffer[0], &result[round][0]);
	}
	position += mfcc_buffer_size;
	round++;
	
	// And finally run zeros until done
	while (position < frames) {
		if (run_plugin) {
			plugin->run (&zero_buffer[0], &analysis_buffer[0], mfcc_buffer_size);
			processor.run (&analysis_buffer[0], &result[round][0]);
		} else {
			processor.run (&zero_buffer[0], &result[round][0]);
		}
		position += mfcc_buffer_size;
		round++;
	}
}

float
MfccEvaluator::euclidean_distance (Data const & a, Data const & b)
{
	Data::const_iterator a_it, b_it;
	float sum = 0.0;
	
	for (a_it = a.begin(), b_it = b.begin(); a_it != a.end() && b_it != b.end(); ++a_it, ++b_it) {
		sum += powf ((*a_it - *b_it), 2);
	}
	
	return sqrt (sum);
}

} // namespace ReverbTuner
