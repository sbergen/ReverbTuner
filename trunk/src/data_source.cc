#include "reverbtuner/data_source.h"

#include <stdexcept>
#include <sndfile.hh>

namespace ReverbTuner {

DataSource::DataSource (std::string const & dry_filefilename, std::string const & target_filefilename, Plugin & plugin)
  : plugin (plugin)
{
	SndfileHandle dry_file (dry_filefilename);
	SndfileHandle target_file (target_filefilename);
	
	samplerate = dry_file.samplerate ();
	
	if (dry_file.channels() != 1 || target_file.channels() != 1) {
		throw std::invalid_argument ("dry or target file is not mono");
	}
	
	read_file (dry_file, dry_data);
	read_file (target_file, target_data);
}

DataSource::~DataSource ()
{
	
}

void
DataSource::read_file (SndfileHandle & file, Data & dest)
{
	unsigned frames = file.frames();
	dest.resize (frames);
	file.read (&dest[0], frames);
}

} // namespace ReverbTuner
