///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/statbmp.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statusbr.h>
#include <wx/timer.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class CMainFrame
///////////////////////////////////////////////////////////////////////////////
class CMainFrame : public wxFrame
{
	private:

	protected:
		wxMenuBar* MainMenuBar;
		wxMenu* FileMenu;
		wxMenu* ViewMenu;
		wxMenu* OptionsMenu;
		wxMenu* HelpMenu;
		wxStaticBitmap* BackgroundBitmap;
		wxButton* BtnConfigureSounds;
		wxStatusBar* StatusBar;
		wxTimer RefreshTimer;

		// Virtual event handlers, override them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnMenuClearPresets( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuLoadPresets( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuSavePresets( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuSavePresetsAs( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnQuit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnViewVirtualKeyboard( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuConfigureSynth( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuConfigureMIDI( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnConfigureSoundFontsPresets( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRefreshTimer( wxTimerEvent& event ) { event.Skip(); }


	public:

		CMainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("wxFluid"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 981,297 ), long style = wxCAPTION|wxCLOSE_BOX|wxSYSTEM_MENU|wxTAB_TRAVERSAL );

		~CMainFrame();

};

///////////////////////////////////////////////////////////////////////////////
/// Class AboutDialog
///////////////////////////////////////////////////////////////////////////////
class AboutDialog : public wxDialog
{
	private:

	protected:
		wxStaticText* name;
		wxStaticText* wxcredit;
		wxStaticText* copyright;
		wxButton* BtnClose;

	public:
		wxStaticBitmap* ImageLogo;
		wxStaticText* AppVersion;
		wxStaticText* FluidSynthVersion;

		AboutDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("About wxFluid..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 363,475 ), long style = wxDEFAULT_DIALOG_STYLE );

		~AboutDialog();

};

