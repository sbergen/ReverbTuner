#ifndef REVERB_TUBER_GUI_ASSISTANT_FILE_PAGE_H
#define REVERB_TUBER_GUI_ASSISTANT_FILE_PAGE_H

#include <boost/shared_ptr.hpp>

#include <gtkmm.h>

namespace ReverbTuner {
	class DataSource;
}

class AssistantFilePage : public Gtk::VBox
{
  public:
	AssistantFilePage ();
	~AssistantFilePage ();
	
	typedef boost::shared_ptr<ReverbTuner::DataSource> DataSourcePtr;
	DataSourcePtr get_data_source () { return data_source; }
	
  private:

	void load_files ();
	
	class FileChooser : public Gtk::HBox
	{
	  public:
		FileChooser (Glib::ustring const & description, Glib::ustring const & default_file);
		~FileChooser ();
		
		Glib::ustring get_filename () const { return entry.get_text (); }
		
	  private:
		void browse ();
		
		Gtk::VBox   entry_box;
		Gtk::Label  entry_label;
		Gtk::Entry  entry;
		
		Gtk::Button browse_button;
	};

	// GUI components
	FileChooser wet_file_chooser;
	FileChooser dry_file_chooser;
	Gtk::Button load_button;
	
	// Data
	DataSourcePtr data_source;
};

#endif // REVERB_TUBER_GUI_ASSISTANT_FILE_PAGE_H
