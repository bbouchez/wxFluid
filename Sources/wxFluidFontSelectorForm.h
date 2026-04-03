///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/font.h>
#include <wx/grid.h>
#include <wx/gdicmn.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class CSoundFontPresetManagerDialog
///////////////////////////////////////////////////////////////////////////////
class CSoundFontPresetManagerDialog : public wxDialog
{
	private:

	protected:
		wxNotebook* SFPresetNoteBook;
		wxPanel* SoundFontPanel;
		wxButton* BtnLoad;
		wxButton* BtnUnLoad;
		wxButton* BtnTestSFLib;
		wxPanel* PresetPanel;
		wxGrid* PresetGrid;

		// Virtual event handlers, override them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnNotebookPageChanged( wxNotebookEvent& event ) { event.Skip(); }
		virtual void OnSFGridDblClick( wxGridEvent& event ) { event.Skip(); }
		virtual void OnSFGridSelected( wxGridEvent& event ) { event.Skip(); }
		virtual void OnBtnLoad( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnUnload( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBtnTestSFLib( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPresetGridChange( wxGridEvent& event ) { event.Skip(); }
		virtual void OnPresetGridSelected( wxGridEvent& event ) { event.Skip(); }


	public:
		wxGrid* SFGrid;

		CSoundFontPresetManagerDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("SoundFont / Preset Manager"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 864,480 ), long style = wxDEFAULT_DIALOG_STYLE );

		~CSoundFontPresetManagerDialog();

};

