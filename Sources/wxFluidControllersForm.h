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
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class CKnobFrame
///////////////////////////////////////////////////////////////////////////////
class CKnobFrame : public wxFrame
{
	private:

	protected:
		wxStaticText* m_staticText9;
		wxChoice* ChannelChoice;
		wxPanel* KnobPanel1;
		wxPanel* KnobPanel2;
		wxPanel* KnobPanel3;
		wxPanel* KnobPanel4;
		wxStaticText* KnobLabel1;
		wxStaticText* KnobLabel2;
		wxStaticText* KnobLabel3;
		wxStaticText* KnobLabel4;
		wxPanel* KnobPanel5;
		wxPanel* KnobPanel6;
		wxPanel* KnobPanel7;
		wxPanel* KnobPanel8;
		wxStaticText* KnobLabel5;
		wxStaticText* KnobLabel6;
		wxStaticText* KnobLabel7;
		wxStaticText* KnobLabel8;

		// Virtual event handlers, override them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }


	public:

		CKnobFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("MIDI Controllers"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 336,296 ), long style = wxCAPTION|wxCLOSE_BOX|wxTAB_TRAVERSAL );

		~CKnobFrame();

};

