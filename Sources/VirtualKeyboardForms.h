///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class CVirtualKeyboardFrame
///////////////////////////////////////////////////////////////////////////////
class CVirtualKeyboardFrame : public wxFrame
{
	private:

	protected:
		wxPanel* KeyPanel;
		wxStaticText* m_staticText24;
		wxSpinCtrl* SpinOctave;
		wxStaticText* m_staticText25;
		wxChoice* OnVelocityChoice;
		wxStaticText* m_staticText7;
		wxChoice* ChannelChoice;

		// Virtual event handlers, override them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnPaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnMouseLeave( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnLeftMouseDown( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnLeftMouseUp( wxMouseEvent& event ) { event.Skip(); }


	public:

		CVirtualKeyboardFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Virtual keyboard"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 820,263 ), long style = wxCAPTION|wxCLOSE_BOX|wxFRAME_TOOL_WINDOW|wxTAB_TRAVERSAL );

		~CVirtualKeyboardFrame();

};

