///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wxFluidMainForms.h"

///////////////////////////////////////////////////////////////////////////

CMainFrame::CMainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	MainMenuBar = new wxMenuBar( 0 );
	FileMenu = new wxMenu();
	wxMenuItem* MenuClearPresets;
	MenuClearPresets = new wxMenuItem( FileMenu, wxID_ANY, wxString( wxT("Clear Presets") ) + wxT('\t') + wxT("Ctrl+N"), wxEmptyString, wxITEM_NORMAL );
	FileMenu->Append( MenuClearPresets );

	wxMenuItem* MenuLoadPresets;
	MenuLoadPresets = new wxMenuItem( FileMenu, wxID_ANY, wxString( wxT("Load Presets") ) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL );
	FileMenu->Append( MenuLoadPresets );

	FileMenu->AppendSeparator();

	wxMenuItem* MenuSavePresets;
	MenuSavePresets = new wxMenuItem( FileMenu, wxID_ANY, wxString( wxT("Save Presets") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	FileMenu->Append( MenuSavePresets );

	wxMenuItem* MenuSavePresetsAs;
	MenuSavePresetsAs = new wxMenuItem( FileMenu, wxID_ANY, wxString( wxT("Save Presets As...") ) , wxEmptyString, wxITEM_NORMAL );
	FileMenu->Append( MenuSavePresetsAs );

	FileMenu->AppendSeparator();

	wxMenuItem* FileExitMenu;
	FileExitMenu = new wxMenuItem( FileMenu, wxID_ANY, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	FileMenu->Append( FileExitMenu );

	MainMenuBar->Append( FileMenu, wxT("&File") );

	ViewMenu = new wxMenu();
	wxMenuItem* ViewVirtualKeyboardPanel;
	ViewVirtualKeyboardPanel = new wxMenuItem( ViewMenu, wxID_ANY, wxString( wxT("Virtual keyboard") ) + wxT('\t') + wxT("Ctrl+K"), wxEmptyString, wxITEM_NORMAL );
	ViewMenu->Append( ViewVirtualKeyboardPanel );

	MainMenuBar->Append( ViewMenu, wxT("&View") );

	OptionsMenu = new wxMenu();
	wxMenuItem* MenuConfigureSynth;
	MenuConfigureSynth = new wxMenuItem( OptionsMenu, wxID_ANY, wxString( wxT("Synthesizer Configuration") ) , wxEmptyString, wxITEM_NORMAL );
	OptionsMenu->Append( MenuConfigureSynth );

	wxMenuItem* MenuConfigureMIDI;
	MenuConfigureMIDI = new wxMenuItem( OptionsMenu, wxID_ANY, wxString( wxT("MIDI Configuration") ) , wxEmptyString, wxITEM_NORMAL );
	OptionsMenu->Append( MenuConfigureMIDI );

	MainMenuBar->Append( OptionsMenu, wxT("&Options") );

	HelpMenu = new wxMenu();
	wxMenuItem* HelpAboutMenu;
	HelpAboutMenu = new wxMenuItem( HelpMenu, wxID_ANY, wxString( wxT("&About") ) + wxT('\t') + wxT("F1"), wxEmptyString, wxITEM_NORMAL );
	HelpMenu->Append( HelpAboutMenu );

	MainMenuBar->Append( HelpMenu, wxT("&Help") );

	this->SetMenuBar( MainMenuBar );

	wxBoxSizer* TopSizer;
	TopSizer = new wxBoxSizer( wxVERTICAL );

	BackgroundBitmap = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 950,170 ), 0 );
	BackgroundBitmap->SetMinSize( wxSize( 950,170 ) );
	BackgroundBitmap->SetMaxSize( wxSize( 950,170 ) );

	TopSizer->Add( BackgroundBitmap, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	BtnConfigureSounds = new wxButton( this, wxID_ANY, wxT("Configure SoundFonts / Presets"), wxDefaultPosition, wxDefaultSize, 0 );
	TopSizer->Add( BtnConfigureSounds, 0, wxALL, 5 );


	this->SetSizer( TopSizer );
	this->Layout();
	StatusBar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );
	RefreshTimer.SetOwner( this, wxID_ANY );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CMainFrame::OnClose ) );
	FileMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CMainFrame::OnMenuClearPresets ), this, MenuClearPresets->GetId());
	FileMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CMainFrame::OnMenuLoadPresets ), this, MenuLoadPresets->GetId());
	FileMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CMainFrame::OnMenuSavePresets ), this, MenuSavePresets->GetId());
	FileMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CMainFrame::OnMenuSavePresetsAs ), this, MenuSavePresetsAs->GetId());
	FileMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CMainFrame::OnQuit ), this, FileExitMenu->GetId());
	ViewMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CMainFrame::OnViewVirtualKeyboard ), this, ViewVirtualKeyboardPanel->GetId());
	OptionsMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CMainFrame::OnMenuConfigureSynth ), this, MenuConfigureSynth->GetId());
	OptionsMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CMainFrame::OnMenuConfigureMIDI ), this, MenuConfigureMIDI->GetId());
	HelpMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CMainFrame::OnAbout ), this, HelpAboutMenu->GetId());
	BtnConfigureSounds->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CMainFrame::OnConfigureSoundFontsPresets ), NULL, this );
	this->Connect( wxID_ANY, wxEVT_TIMER, wxTimerEventHandler( CMainFrame::OnRefreshTimer ) );
}

CMainFrame::~CMainFrame()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CMainFrame::OnClose ) );
	BtnConfigureSounds->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CMainFrame::OnConfigureSoundFontsPresets ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_TIMER, wxTimerEventHandler( CMainFrame::OnRefreshTimer ) );

}

AboutDialog::AboutDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* container;
	container = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* sizer;
	sizer = new wxBoxSizer( wxVERTICAL );

	ImageLogo = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxSize( 278,253 ), 0 );
	sizer->Add( ImageLogo, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	name = new wxStaticText( this, wxID_ANY, wxT("wxFluid"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	name->Wrap( -1 );
	name->SetFont( wxFont( 18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );

	sizer->Add( name, 0, wxALIGN_CENTER_HORIZONTAL|wxBOTTOM|wxEXPAND, 10 );

	AppVersion = new wxStaticText( this, wxID_ANY, wxT("Application Version"), wxDefaultPosition, wxDefaultSize, 0 );
	AppVersion->Wrap( -1 );
	sizer->Add( AppVersion, 0, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL, 5 );

	FluidSynthVersion = new wxStaticText( this, wxID_ANY, wxT("FluidSynth engine Version"), wxDefaultPosition, wxDefaultSize, 0 );
	FluidSynthVersion->Wrap( -1 );
	sizer->Add( FluidSynthVersion, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	wxcredit = new wxStaticText( this, wxID_ANY, wxT("This application is built using wxWidgets 3.2"), wxDefaultPosition, wxDefaultSize, 0 );
	wxcredit->Wrap( -1 );
	sizer->Add( wxcredit, 0, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL, 5 );

	copyright = new wxStaticText( this, wxID_ANY, wxT("Copyright Benoit BOUCHEZ (BEB)"), wxDefaultPosition, wxDefaultSize, 0 );
	copyright->Wrap( -1 );
	sizer->Add( copyright, 0, wxALIGN_CENTER_HORIZONTAL|wxBOTTOM, 10 );

	BtnClose = new wxButton( this, wxID_CANCEL, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	sizer->Add( BtnClose, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	container->Add( sizer, 1, wxEXPAND, 5 );


	this->SetSizer( container );
	this->Layout();

	this->Centre( wxBOTH );
}

AboutDialog::~AboutDialog()
{
}
