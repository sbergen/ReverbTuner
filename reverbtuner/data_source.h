#ifndef REVERB_TUNER_DATA_SOURCE_H
#define REVERB_TUNER_DATA_SOURCE_H

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

class SndfileHandle;

namespace ReverbTuner {

class Plugin;

/** Stores data needed for evaluation, namely the dry and target files.
  * Evaluators process the dry file and compare it to the target file.
  */
class DataSource
{
  public:
	
	typedef std::vector<float> Data;
	
	/// Throws std::invalid_argument if files are not valid in some way
	DataSource (std::string const & dry_filename, std::string const & target_filename);
	~DataSource ();
	
	Data const & get_dry_data () const { return dry_data; }
	Data const & get_target_data () const { return target_data; }
	double get_samplerate () const { return samplerate; }
	
	void set_plugin (boost::shared_ptr<Plugin> plugin_) { plugin = plugin_; }
	boost::shared_ptr<Plugin const> get_plugin () const { return plugin; }
	
  private:
	
	void read_file (SndfileHandle & file, Data & dest);
	
	Data dry_data;
	Data target_data;
	boost::shared_ptr<Plugin> plugin;
	
	double samplerate;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_DATA_SOURCE_H