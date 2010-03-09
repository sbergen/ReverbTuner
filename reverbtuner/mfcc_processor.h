#ifndef REVERB_TUNER_MFCC_PROCESSOR_H
#define REVERB_TUNER_MFCC_PROCESSOR_H

#include <aubio/aubio.h>

namespace ReverbTuner {

class MfccProcessor
{
  public:
	
	MfccProcessor (unsigned buffer_size, unsigned hop_size, unsigned filters, unsigned coefs, unsigned samplerate);
	~MfccProcessor ();

	void run (float const * data, float * result);
	
  private:
	
	// These are allocated
	aubio_mfcc_t * mfcc;
	aubio_pvoc_t * pv;
	cvec_t * pv_buf;
	
	// These vectors are only set to point to data
	fvec_t input_buf;
	float * input_data;
	
	fvec_t coef_buf;
	float * coef_data;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_MFCC_PROCESSOR_H
