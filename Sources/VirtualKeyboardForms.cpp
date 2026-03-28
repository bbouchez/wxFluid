///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "VirtualKeyboardForms.h"

///////////////////////////////////////////////////////////////////////////

CVirtualKeyboardFrame::CVirtualKeyboardFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* MainSizer;
	MainSizer = new wxBoxSizer( wxVERTICAL );

	KeyPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 792,154 ), wxTAB_TRAVERSAL );
	MainSizer->Add( KeyPanel, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	wxBoxSizer* BtnSizer;
	BtnSizer = new wxBoxSizer( wxHORIZONTAL );

	m_staticText24 = new wxStaticText( this, wxID_ANY, wxT("Octave"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( -1 );
	BtnSizer->Add( m_staticText24, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	SpinOctave = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -2, 6, 0 );
	BtnSizer->Add( SpinOctave, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText25 = new wxStaticText( this, wxID_ANY, wxT("On Velocity"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText25->Wrap( -1 );
	BtnSizer->Add( m_staticText25, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString OnVelocityChoiceChoices;
	OnVelocityChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, OnVelocityChoiceChoices, 0 );
	OnVelocityChoice->SetSelection( 0 );
	BtnSizer->Add( OnVelocityChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	BtnSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText7 = new wxStaticText( this, wxID_ANY, wxT("Channel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	BtnSizer->Add( m_staticText7, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString ChannelChoiceChoices[] = { wxT("1"), wxT("2"), wxT("3"), wxT("4"), wxT("5"), wxT("6"), wxT("7"), wxT("8"), wxT("9"), wxT("10"), wxT("11"), wxT("12"), wxT("13"), wxT("14"), wxT("15"), wxT("16") };
	int ChannelChoiceNChoices = sizeof( ChannelChoiceChoices ) / sizeof( wxString );
	ChannelChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, ChannelChoiceNChoices, ChannelChoiceChoices, 0 );
	ChannelChoice->SetSelection( 0 );
	BtnSizer->Add( ChannelChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	MainSizer->Add( BtnSizer, 1, wxEXPAND, 5 );


	this->SetSizer( MainSizer );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CVirtualKeyboardFrame::OnClose ) );
	this->Connect( wxEVT_PAINT, wxPaintEventHandler( CVirtualKeyboardFrame::OnPaint ) );
	KeyPanel->Connect( wxEVT_LEAVE_WINDOW, wxMouseEventHandler( CVirtualKeyboardFrame::OnMouseLeave ), NULL, this );
	KeyPanel->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( CVirtualKeyboardFrame::OnLeftMouseDown ), NULL, this );
	KeyPanel->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( CVirtualKeyboardFrame::OnLeftMouseUp ), NULL, this );
}

CVirtualKeyboardFrame::~CVirtualKeyboardFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CVirtualKeyboardFrame::OnClose ) );
	this->Disconnect( wxEVT_PAINT, wxPaintEventHandler( CVirtualKeyboardFrame::OnPaint ) );
	KeyPanel->Disconnect( wxEVT_LEAVE_WINDOW, wxMouseEventHandler( CVirtualKeyboardFrame::OnMouseLeave ), NULL, this );
	KeyPanel->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( CVirtualKeyboardFrame::OnLeftMouseDown ), NULL, this );
	KeyPanel->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( CVirtualKeyboardFrame::OnLeftMouseUp ), NULL, this );

}
