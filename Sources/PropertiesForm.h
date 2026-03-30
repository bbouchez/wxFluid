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
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/choice.h>
#include <wx/spinctrl.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/textctrl.h>
#include <wx/notebook.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/radiobox.h>
#include <wx/statbox.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class CSynthConfigDialog
///////////////////////////////////////////////////////////////////////////////
class CSynthConfigDialog : public wxDialog
{
	private:

	protected:
		wxNotebook* m_notebook1;
		wxPanel* AudioPanel;
		wxStaticText* m_staticText4;
		wxChoice* AudioDriverChoice;
		wxStaticText* m_staticText5;
		wxChoice* AudioDeviceChoice;
		wxStaticText* m_staticText6;
		wxChoice* SampleRateChoice;
		wxStaticText* m_staticText8;
		wxChoice* AudioBufferSizeChoice;
		wxStaticText* m_staticText9;
		wxSpinCtrl* AudioPeriods;
		wxStaticText* TxtWASAPI;
		wxCheckBox* ChkAudioWasapiExclusive;
		wxPanel* SynthPanel;
		wxStaticText* m_staticText19;
		wxSpinCtrl* SynthChannelsSpin;
		wxStaticText* m_staticText21;
		wxSpinCtrl* SynthAudioGroupsSpin;
		wxStaticText* m_staticText22;
		wxTextCtrl* SynthEditPolyphony;
		wxButton* BtnOK;
		wxButton* BtnCancel;

		// Virtual event handlers, override them in your derived class
		virtual void OnAudioDriverChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOKBtn( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCancelBtn( wxCommandEvent& event ) { event.Skip(); }


	public:

		CSynthConfigDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Engine Configuration"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 610,343 ), long style = wxDEFAULT_DIALOG_STYLE );

		~CSynthConfigDialog();

};

///////////////////////////////////////////////////////////////////////////////
/// Class CMIDIConfigDialog
///////////////////////////////////////////////////////////////////////////////
class CMIDIConfigDialog : public wxDialog
{
	private:

	protected:
		wxPanel* m_panel4;
		wxStaticText* m_staticText12;
		wxCheckBox* ChkUseMIDI;
		wxStaticText* m_staticText14;
		wxChoice* MIDIDeviceChoice;
		wxStaticText* m_staticText15;
		wxSpinCtrl* MIDIChannelsSpin;
		wxStaticText* m_staticText18;
		wxChoice* MIDIBankSelectChoice;
		wxRadioBox* RadioNetworkMIDIMode;
		wxStaticText* m_staticText141;
		wxTextCtrl* EditLocalUDPPort;
		wxCheckBox* ChkSessionInitiator;
		wxStaticText* m_staticText151;
		wxTextCtrl* EditDeviceIP;
		wxStaticText* m_staticText152;
		wxTextCtrl* EditDeviceUDPPort;
		wxButton* BtnOK;
		wxButton* BtnCancel;

		// Virtual event handlers, override them in your derived class
		virtual void OnOKBtn( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCancelBtn( wxCommandEvent& event ) { event.Skip(); }


	public:

		CMIDIConfigDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("MIDI Configuration"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 610,428 ), long style = wxDEFAULT_DIALOG_STYLE );

		~CMIDIConfigDialog();

};

