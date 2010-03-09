#include "reverbtuner/mfcc_processor.h"

namespace ReverbTuner {

MfccProcessor::MfccProcessor(unsigned buffer_size, unsigned hop_size, unsigned filters, unsigned coefs, unsigned samplerate)
{
	unsigned const channels = 1;
	pv = new_aubio_pvoc (buffer_size, hop_size, channels);
	pv_buf = new_cvec (buffer_size, channels);
	mfcc = new_aubio_mfcc (buffer_size, filters, coefs, samplerate);
	
	input_buf.length = hop_size;
	input_buf.channels = channels;
	input_buf.data = &input_data;
	
	coef_buf.length = coefs;
	coef_buf.channels = channels;
	coef_buf.data = &coef_data;
}

MfccProcessor::~MfccProcessor()
{
	del_aubio_pvoc (pv);
	del_aubio_mfcc(mfcc);
	del_cvec (pv_buf);
}

void
MfccProcessor::run (float const * data, float * result)
{
	input_data = const_cast<float *> (data);
	coef_data = result;
	
	aubio_pvoc_do (pv, &input_buf, pv_buf);
	aubio_mfcc_do (mfcc, pv_buf, &coef_buf);
}

} // namespace ReverbTuner
