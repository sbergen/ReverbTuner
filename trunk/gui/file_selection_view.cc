#include "file_selection_view.h"

#include "reverbtuner/data_source.h"

AssistantFilePage::AssistantFilePage ()
  : wet_file_chooser ("Wet impulse file:", "samples/sample.wav")
  , dry_file_chooser ("Dry impulse file:", "samples/impulse.wav")
  , load_button ("Load files")
{
	pack_start (wet_file_chooser, false, false);
	pack_start (wet_waveform, true, true);
	pack_start (dry_file_chooser, false, false);
	pack_start (dry_waveform, true, true);
	pack_start (load_button, false, false);
	
	load_button.signal_clicked().connect (sigc::mem_fun(*this, &AssistantFilePage::load_files));
}

AssistantFilePage::~AssistantFilePage ()
{

}

void
AssistantFilePage::load_files ()
{
	wet_waveform.reset_data ();
	dry_waveform.reset_data ();
	
	try {
		data_source.reset (new ReverbTuner::DataSource (
			dry_file_chooser.get_filename (), wet_file_chooser.get_filename ()));
	} catch (std::exception & e) {
		Glib::ustring message = "Failed to load files: ";
		message += e.what ();
		Gtk::MessageDialog fail_dialog (message, false, Gtk::MESSAGE_ERROR);
		fail_dialog.run ();
		complete_changed (*this, false);
		return;
	}
	
	ReverbTuner::DataSource::Data const & wet_data = data_source->get_target_data ();
	ReverbTuner::DataSource::Data const & dry_data = data_source->get_dry_data ();
	
	wet_waveform.set_data (wet_data, wet_data.size());
	dry_waveform.set_data (dry_data, wet_data.size());
	
	complete_changed (*this, true);
}

/*** File chooser ***/

AssistantFilePage::FileChooser::FileChooser (Glib::ustring const & description, Glib::ustring const & default_file)
  : entry_label (description, Gtk::ALIGN_LEFT)
{
	// Entry entry and button
	entry.set_text (default_file);
	entry_box.pack_start (entry, true, true);
	entry_box.pack_start (browse_button, false, false);
	
	// Button
	browse_button.set_label ("Browse");
	browse_button.signal_clicked().connect (sigc::mem_fun(*this, &AssistantFilePage::FileChooser::browse));
	
	// Pack
	pack_start (entry_label, false, false);
	pack_start (entry_box);
}

AssistantFilePage::FileChooser::~FileChooser ()
{

}

void
AssistantFilePage::FileChooser::browse ()
{
	Gtk::FileChooserDialog dialog ("Choose file", Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_filename (entry.get_text ());
	dialog.add_button (Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button (Gtk::Stock::OK, Gtk::RESPONSE_OK);

	int result = dialog.run();

	if (result == Gtk::RESPONSE_OK) {
		Glib::ustring filename = dialog.get_filename ();

		if (filename.length ()) {
			entry.set_text (filename);
		}
	}
}
