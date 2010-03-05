#ifndef REVERB_TUNER_DATA_SOURCE_H
#define REVERB_TUNER_DATA_SOURCE_H

#include <string>
#include <vector>

namespace ReverbTuner {

/** Stores data needed for evaluation, namely the dry and target files.
  * Evaluators process the dry file and compare it to the target file.
  */
class DataSource
{
  public:
	
	typedef std::vector<float> Data;
	
	DataSource (std::string const & dry_file, std::string const & target_file);
	~DataSource ();
	
	Data const & get_dry_data() const { return dry_data; }
	Data const & get_target_data()const { return target_data; }
	
  private:
	Data dry_data;
	Data target_data;
};

} // namespace ReverbTuner

#endif // REVERB_TUNER_DATA_SOURCE_H