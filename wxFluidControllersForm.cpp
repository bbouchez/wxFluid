///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wxFluidControllersForm.h"

///////////////////////////////////////////////////////////////////////////

CKnobFrame::CKnobFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* TopSizer;
	TopSizer = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* ChannelSizer;
	ChannelSizer = new wxBoxSizer( wxHORIZONTAL );

	m_staticText9 = new wxStaticText( this, wxID_ANY, wxT("Channel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	ChannelSizer->Add( m_staticText9, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString ChannelChoiceChoices[] = { wxT("1"), wxT("2"), wxT("3"), wxT("4"), wxT("5"), wxT("6"), wxT("7"), wxT("8"), wxT("9"), wxT("10"), wxT("11"), wxT("12"), wxT("13"), wxT("14"), wxT("15"), wxT("16") };
	int ChannelChoiceNChoices = sizeof( ChannelChoiceChoices ) / sizeof( wxString );
	ChannelChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, ChannelChoiceNChoices, ChannelChoiceChoices, 0 );
	ChannelChoice->SetSelection( 0 );
	ChannelSizer->Add( ChannelChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	TopSizer->Add( ChannelSizer, 1, wxEXPAND, 5 );

	wxFlexGridSizer* KnobSizer;
	KnobSizer = new wxFlexGridSizer( 4, 4, 0, 0 );
	KnobSizer->SetFlexibleDirection( wxBOTH );
	KnobSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	KnobPanel1 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 66,66 ), wxTAB_TRAVERSAL );
	KnobPanel1->SetMinSize( wxSize( 66,66 ) );
	KnobPanel1->SetMaxSize( wxSize( 66,66 ) );

	KnobSizer->Add( KnobPanel1, 1, wxEXPAND | wxALL, 5 );

	KnobPanel2 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 66,66 ), wxTAB_TRAVERSAL );
	KnobPanel2->SetMinSize( wxSize( 66,66 ) );
	KnobPanel2->SetMaxSize( wxSize( 66,66 ) );

	KnobSizer->Add( KnobPanel2, 1, wxEXPAND | wxALL, 5 );

	KnobPanel3 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 66,66 ), wxTAB_TRAVERSAL );
	KnobPanel3->SetMinSize( wxSize( 66,66 ) );
	KnobPanel3->SetMaxSize( wxSize( 66,66 ) );

	KnobSizer->Add( KnobPanel3, 1, wxEXPAND | wxALL, 5 );

	KnobPanel4 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 66,66 ), wxTAB_TRAVERSAL );
	KnobPanel4->SetMinSize( wxSize( 66,66 ) );
	KnobPanel4->SetMaxSize( wxSize( 66,66 ) );

	KnobSizer->Add( KnobPanel4, 1, wxEXPAND | wxALL, 5 );

	KnobLabel1 = new wxStaticText( this, wxID_ANY, wxT("Cutoff"), wxDefaultPosition, wxDefaultSize, 0 );
	KnobLabel1->Wrap( -1 );
	KnobSizer->Add( KnobLabel1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	KnobLabel2 = new wxStaticText( this, wxID_ANY, wxT("Resonance"), wxDefaultPosition, wxDefaultSize, 0 );
	KnobLabel2->Wrap( -1 );
	KnobSizer->Add( KnobLabel2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	KnobLabel3 = new wxStaticText( this, wxID_ANY, wxT("Attack"), wxDefaultPosition, wxDefaultSize, 0 );
	KnobLabel3->Wrap( -1 );
	KnobSizer->Add( KnobLabel3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	KnobLabel4 = new wxStaticText( this, wxID_ANY, wxT("Decay"), wxDefaultPosition, wxDefaultSize, 0 );
	KnobLabel4->Wrap( -1 );
	KnobSizer->Add( KnobLabel4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	KnobPanel5 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 66,66 ), wxTAB_TRAVERSAL );
	KnobPanel5->SetMinSize( wxSize( 66,66 ) );
	KnobPanel5->SetMaxSize( wxSize( 66,66 ) );

	KnobSizer->Add( KnobPanel5, 1, wxEXPAND | wxALL, 5 );

	KnobPanel6 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 66,66 ), wxTAB_TRAVERSAL );
	KnobPanel6->SetMinSize( wxSize( 66,66 ) );
	KnobPanel6->SetMaxSize( wxSize( 66,66 ) );

	KnobSizer->Add( KnobPanel6, 1, wxEXPAND | wxALL, 5 );

	KnobPanel7 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 66,66 ), wxTAB_TRAVERSAL );
	KnobPanel7->SetMinSize( wxSize( 66,66 ) );
	KnobPanel7->SetMaxSize( wxSize( 66,66 ) );

	KnobSizer->Add( KnobPanel7, 1, wxEXPAND | wxALL, 5 );

	KnobPanel8 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxSize( 66,66 ), wxTAB_TRAVERSAL );
	KnobPanel8->SetMinSize( wxSize( 66,66 ) );
	KnobPanel8->SetMaxSize( wxSize( 66,66 ) );

	KnobSizer->Add( KnobPanel8, 1, wxEXPAND | wxALL, 5 );

	KnobLabel5 = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	KnobLabel5->Wrap( -1 );
	KnobSizer->Add( KnobLabel5, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	KnobLabel6 = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	KnobLabel6->Wrap( -1 );
	KnobSizer->Add( KnobLabel6, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	KnobLabel7 = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	KnobLabel7->Wrap( -1 );
	KnobSizer->Add( KnobLabel7, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	KnobLabel8 = new wxStaticText( this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	KnobLabel8->Wrap( -1 );
	KnobSizer->Add( KnobLabel8, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	TopSizer->Add( KnobSizer, 1, wxEXPAND, 5 );


	this->SetSizer( TopSizer );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CKnobFrame::OnClose ) );
}

CKnobFrame::~CKnobFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CKnobFrame::OnClose ) );

}
