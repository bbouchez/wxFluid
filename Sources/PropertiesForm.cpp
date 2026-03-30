///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "PropertiesForm.h"

///////////////////////////////////////////////////////////////////////////

CSynthConfigDialog::CSynthConfigDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* TopSizer;
	TopSizer = new wxBoxSizer( wxVERTICAL );

	m_notebook1 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	AudioPanel = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer5;
	fgSizer5 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer5->SetFlexibleDirection( wxBOTH );
	fgSizer5->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText4 = new wxStaticText( AudioPanel, wxID_ANY, wxT("Driver"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	fgSizer5->Add( m_staticText4, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString AudioDriverChoiceChoices[] = { wxT(" ") };
	int AudioDriverChoiceNChoices = sizeof( AudioDriverChoiceChoices ) / sizeof( wxString );
	AudioDriverChoice = new wxChoice( AudioPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, AudioDriverChoiceNChoices, AudioDriverChoiceChoices, 0 );
	AudioDriverChoice->SetSelection( 0 );
	AudioDriverChoice->SetMinSize( wxSize( 100,-1 ) );

	fgSizer5->Add( AudioDriverChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText5 = new wxStaticText( AudioPanel, wxID_ANY, wxT("Device"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer5->Add( m_staticText5, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString AudioDeviceChoiceChoices[] = { wxT(" ") };
	int AudioDeviceChoiceNChoices = sizeof( AudioDeviceChoiceChoices ) / sizeof( wxString );
	AudioDeviceChoice = new wxChoice( AudioPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, AudioDeviceChoiceNChoices, AudioDeviceChoiceChoices, 0 );
	AudioDeviceChoice->SetSelection( 0 );
	AudioDeviceChoice->SetMinSize( wxSize( 400,-1 ) );

	fgSizer5->Add( AudioDeviceChoice, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText6 = new wxStaticText( AudioPanel, wxID_ANY, wxT("Sample Rate"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer5->Add( m_staticText6, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString SampleRateChoiceChoices[] = { wxT("44.1 kHz"), wxT("48 kHz"), wxT("88.2 kHz"), wxT("96 kHz"), wxT("192 kHz") };
	int SampleRateChoiceNChoices = sizeof( SampleRateChoiceChoices ) / sizeof( wxString );
	SampleRateChoice = new wxChoice( AudioPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, SampleRateChoiceNChoices, SampleRateChoiceChoices, 0 );
	SampleRateChoice->SetSelection( 0 );
	fgSizer5->Add( SampleRateChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText8 = new wxStaticText( AudioPanel, wxID_ANY, wxT("Buffers size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	fgSizer5->Add( m_staticText8, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString AudioBufferSizeChoiceChoices[] = { wxT("64"), wxT("128"), wxT("256"), wxT("512"), wxT("1024"), wxT("2048"), wxT("4096"), wxT("8192") };
	int AudioBufferSizeChoiceNChoices = sizeof( AudioBufferSizeChoiceChoices ) / sizeof( wxString );
	AudioBufferSizeChoice = new wxChoice( AudioPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, AudioBufferSizeChoiceNChoices, AudioBufferSizeChoiceChoices, 0 );
	AudioBufferSizeChoice->SetSelection( 0 );
	fgSizer5->Add( AudioBufferSizeChoice, 0, wxALL, 5 );

	m_staticText9 = new wxStaticText( AudioPanel, wxID_ANY, wxT("Buffers"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	fgSizer5->Add( m_staticText9, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	AudioPeriods = new wxSpinCtrl( AudioPanel, wxID_ANY, wxT("8"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 2, 64, 8 );
	fgSizer5->Add( AudioPeriods, 0, wxALL, 5 );

	TxtWASAPI = new wxStaticText( AudioPanel, wxID_ANY, wxT("WASAPI Exclusive mode"), wxDefaultPosition, wxDefaultSize, 0 );
	TxtWASAPI->Wrap( -1 );
	fgSizer5->Add( TxtWASAPI, 0, wxALL, 5 );

	ChkAudioWasapiExclusive = new wxCheckBox( AudioPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer5->Add( ChkAudioWasapiExclusive, 0, wxALL, 5 );


	AudioPanel->SetSizer( fgSizer5 );
	AudioPanel->Layout();
	fgSizer5->Fit( AudioPanel );
	m_notebook1->AddPage( AudioPanel, wxT("Audio"), true );
	SynthPanel = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer7;
	fgSizer7 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer7->SetFlexibleDirection( wxBOTH );
	fgSizer7->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText19 = new wxStaticText( SynthPanel, wxID_ANY, wxT("Stereo Channels Pairs"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	fgSizer7->Add( m_staticText19, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	SynthChannelsSpin = new wxSpinCtrl( SynthPanel, wxID_ANY, wxT("1"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 128, 1 );
	fgSizer7->Add( SynthChannelsSpin, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText21 = new wxStaticText( SynthPanel, wxID_ANY, wxT("Audio Groups"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	fgSizer7->Add( m_staticText21, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	SynthAudioGroupsSpin = new wxSpinCtrl( SynthPanel, wxID_ANY, wxT("1"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 128, 1 );
	fgSizer7->Add( SynthAudioGroupsSpin, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText22 = new wxStaticText( SynthPanel, wxID_ANY, wxT("Polyphony"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText22->Wrap( -1 );
	fgSizer7->Add( m_staticText22, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	SynthEditPolyphony = new wxTextCtrl( SynthPanel, wxID_ANY, wxT("256"), wxDefaultPosition, wxDefaultSize, 0 );
	#ifdef __WXGTK__
	if ( !SynthEditPolyphony->HasFlag( wxTE_MULTILINE ) )
	{
	SynthEditPolyphony->SetMaxLength( 5 );
	}
	#else
	SynthEditPolyphony->SetMaxLength( 5 );
	#endif
	fgSizer7->Add( SynthEditPolyphony, 0, wxALL, 5 );


	SynthPanel->SetSizer( fgSizer7 );
	SynthPanel->Layout();
	fgSizer7->Fit( SynthPanel );
	m_notebook1->AddPage( SynthPanel, wxT("Synthesizer"), false );

	TopSizer->Add( m_notebook1, 1, wxEXPAND | wxALL, 5 );

	wxBoxSizer* BtnSizer;
	BtnSizer = new wxBoxSizer( wxHORIZONTAL );

	BtnOK = new wxButton( this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	BtnSizer->Add( BtnOK, 0, wxALL, 5 );

	BtnCancel = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	BtnSizer->Add( BtnCancel, 0, wxALL, 5 );


	TopSizer->Add( BtnSizer, 0, wxALIGN_RIGHT, 5 );


	this->SetSizer( TopSizer );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	AudioDriverChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CSynthConfigDialog::OnAudioDriverChoice ), NULL, this );
	BtnOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSynthConfigDialog::OnOKBtn ), NULL, this );
	BtnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSynthConfigDialog::OnCancelBtn ), NULL, this );
}

CSynthConfigDialog::~CSynthConfigDialog()
{
	// Disconnect Events
	AudioDriverChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CSynthConfigDialog::OnAudioDriverChoice ), NULL, this );
	BtnOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSynthConfigDialog::OnOKBtn ), NULL, this );
	BtnCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSynthConfigDialog::OnCancelBtn ), NULL, this );

}

CMIDIConfigDialog::CMIDIConfigDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* TopSizer;
	TopSizer = new wxBoxSizer( wxVERTICAL );

	m_panel4 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME|wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText12 = new wxStaticText( m_panel4, wxID_ANY, wxT("Use MIDI Input"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	fgSizer6->Add( m_staticText12, 0, wxALL, 5 );

	ChkUseMIDI = new wxCheckBox( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( ChkUseMIDI, 0, wxALL, 5 );

	m_staticText14 = new wxStaticText( m_panel4, wxID_ANY, wxT("Device"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14->Wrap( -1 );
	fgSizer6->Add( m_staticText14, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxArrayString MIDIDeviceChoiceChoices;
	MIDIDeviceChoice = new wxChoice( m_panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, MIDIDeviceChoiceChoices, 0 );
	MIDIDeviceChoice->SetSelection( 0 );
	MIDIDeviceChoice->SetMinSize( wxSize( 400,-1 ) );

	fgSizer6->Add( MIDIDeviceChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText15 = new wxStaticText( m_panel4, wxID_ANY, wxT("Channels"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText15->Wrap( -1 );
	fgSizer6->Add( m_staticText15, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	MIDIChannelsSpin = new wxSpinCtrl( m_panel4, wxID_ANY, wxT("16"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 16, 256, 16 );
	fgSizer6->Add( MIDIChannelsSpin, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText18 = new wxStaticText( m_panel4, wxID_ANY, wxT("Bank Select Mode"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	fgSizer6->Add( m_staticText18, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	wxString MIDIBankSelectChoiceChoices[] = { wxT("GM"), wxT("GS"), wxT("MMA"), wxT("XG") };
	int MIDIBankSelectChoiceNChoices = sizeof( MIDIBankSelectChoiceChoices ) / sizeof( wxString );
	MIDIBankSelectChoice = new wxChoice( m_panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, MIDIBankSelectChoiceNChoices, MIDIBankSelectChoiceChoices, 0 );
	MIDIBankSelectChoice->SetSelection( 0 );
	fgSizer6->Add( MIDIBankSelectChoice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	m_panel4->SetSizer( fgSizer6 );
	m_panel4->Layout();
	fgSizer6->Fit( m_panel4 );
	TopSizer->Add( m_panel4, 1, wxEXPAND | wxALL, 5 );

	wxStaticBoxSizer* NetworkMIDISizer;
	NetworkMIDISizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Network MIDI") ), wxVERTICAL );

	wxString RadioNetworkMIDIModeChoices[] = { wxT("Disabled"), wxT("RTP-MIDI"), wxT("NetUMP") };
	int RadioNetworkMIDIModeNChoices = sizeof( RadioNetworkMIDIModeChoices ) / sizeof( wxString );
	RadioNetworkMIDIMode = new wxRadioBox( NetworkMIDISizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, RadioNetworkMIDIModeNChoices, RadioNetworkMIDIModeChoices, 3, wxRA_SPECIFY_COLS );
	RadioNetworkMIDIMode->SetSelection( 0 );
	NetworkMIDISizer->Add( RadioNetworkMIDIMode, 0, wxALL, 5 );

	wxFlexGridSizer* fgSizer61;
	fgSizer61 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer61->SetFlexibleDirection( wxBOTH );
	fgSizer61->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText141 = new wxStaticText( NetworkMIDISizer->GetStaticBox(), wxID_ANY, wxT("Local UDP Port"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText141->Wrap( -1 );
	fgSizer61->Add( m_staticText141, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	EditLocalUDPPort = new wxTextCtrl( NetworkMIDISizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	#ifdef __WXGTK__
	if ( !EditLocalUDPPort->HasFlag( wxTE_MULTILINE ) )
	{
	EditLocalUDPPort->SetMaxLength( 5 );
	}
	#else
	EditLocalUDPPort->SetMaxLength( 5 );
	#endif
	fgSizer61->Add( EditLocalUDPPort, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	NetworkMIDISizer->Add( fgSizer61, 1, wxEXPAND, 5 );

	wxStaticBoxSizer* SessionInitiatorSizer;
	SessionInitiatorSizer = new wxStaticBoxSizer( new wxStaticBox( NetworkMIDISizer->GetStaticBox(), wxID_ANY, wxT("Session initiator") ), wxHORIZONTAL );

	ChkSessionInitiator = new wxCheckBox( SessionInitiatorSizer->GetStaticBox(), wxID_ANY, wxT("Active"), wxDefaultPosition, wxDefaultSize, 0 );
	SessionInitiatorSizer->Add( ChkSessionInitiator, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	SessionInitiatorSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	m_staticText151 = new wxStaticText( SessionInitiatorSizer->GetStaticBox(), wxID_ANY, wxT("Remote device IP"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText151->Wrap( -1 );
	SessionInitiatorSizer->Add( m_staticText151, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	EditDeviceIP = new wxTextCtrl( SessionInitiatorSizer->GetStaticBox(), wxID_ANY, wxT("192.168.0.253"), wxDefaultPosition, wxDefaultSize, 0 );
	#ifdef __WXGTK__
	if ( !EditDeviceIP->HasFlag( wxTE_MULTILINE ) )
	{
	EditDeviceIP->SetMaxLength( 16 );
	}
	#else
	EditDeviceIP->SetMaxLength( 16 );
	#endif
	SessionInitiatorSizer->Add( EditDeviceIP, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText152 = new wxStaticText( SessionInitiatorSizer->GetStaticBox(), wxID_ANY, wxT("Remote device Port"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText152->Wrap( -1 );
	SessionInitiatorSizer->Add( m_staticText152, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	EditDeviceUDPPort = new wxTextCtrl( SessionInitiatorSizer->GetStaticBox(), wxID_ANY, wxT("5004"), wxDefaultPosition, wxDefaultSize, 0 );
	#ifdef __WXGTK__
	if ( !EditDeviceUDPPort->HasFlag( wxTE_MULTILINE ) )
	{
	EditDeviceUDPPort->SetMaxLength( 5 );
	}
	#else
	EditDeviceUDPPort->SetMaxLength( 5 );
	#endif
	SessionInitiatorSizer->Add( EditDeviceUDPPort, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	NetworkMIDISizer->Add( SessionInitiatorSizer, 1, wxEXPAND, 5 );


	TopSizer->Add( NetworkMIDISizer, 1, wxEXPAND|wxALL, 5 );

	wxBoxSizer* BtnSizer;
	BtnSizer = new wxBoxSizer( wxHORIZONTAL );

	BtnOK = new wxButton( this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	BtnSizer->Add( BtnOK, 0, wxALL, 5 );

	BtnCancel = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	BtnSizer->Add( BtnCancel, 0, wxALL, 5 );


	TopSizer->Add( BtnSizer, 0, wxALIGN_RIGHT, 5 );


	this->SetSizer( TopSizer );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	BtnOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CMIDIConfigDialog::OnOKBtn ), NULL, this );
	BtnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CMIDIConfigDialog::OnCancelBtn ), NULL, this );
}

CMIDIConfigDialog::~CMIDIConfigDialog()
{
	// Disconnect Events
	BtnOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CMIDIConfigDialog::OnOKBtn ), NULL, this );
	BtnCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CMIDIConfigDialog::OnCancelBtn ), NULL, this );

}
