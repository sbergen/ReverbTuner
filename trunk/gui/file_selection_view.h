#ifndef REVERB_TUNER_GUI_FILE_SELECTION_VIEW_H
#define REVERB_TUNER_GUI_FILE_SELECTION_VIEW_H

#include <boost/shared_ptr.hpp>
#include <gtkmm.h>

#include "reverbtuner/types.h"

#include "waveform.h"

class FileSelectionView : public Gtk::VBox
{
  public:
	FileSelectionView (ReverbTuner::Runner & runner);
	~FileSelectionView ();
	
	sigc::signal<void, Gtk::Widget &, bool> complete_changed;
	
	double file_samplerate ();
	
  private:

	void load_files ();
	
	ReverbTuner::Runner & runner;
	
	class FileChooser : public Gtk::VBox
	{
	  public:
		FileChooser (Glib::ustring const & description, Glib::ustring const & default_file);
		~FileChooser ();
		
		Glib::ustring get_filename () const { return entry.get_text (); }
		
	  private:
		void browse ();
		
		Gtk::Label  entry_label;
		Gtk::HBox   entry_box;
		Gtk::Entry  entry;
		Gtk::Button browse_button;
	};

	// GUI components
	FileChooser wet_file_chooser;
	FileChooser dry_file_chooser;
	Gtk::Button load_button;
	
	Waveform wet_waveform;
	Waveform dry_waveform;
	
	// Data
	ReverbTuner::SharedDataSourcePtr data_source;
};

#endif // REVERB_TUNER_GUI_FILE_SELECTION_VIEW_H
