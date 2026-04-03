///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wxFluidFontSelectorForm.h"

///////////////////////////////////////////////////////////////////////////

CSoundFontPresetManagerDialog::CSoundFontPresetManagerDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* TopSizer;
	TopSizer = new wxBoxSizer( wxVERTICAL );

	SFPresetNoteBook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	SoundFontPanel = new wxPanel( SFPresetNoteBook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );

	SFGrid = new wxGrid( SoundFontPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	SFGrid->CreateGrid( 16, 1 );
	SFGrid->EnableEditing( true );
	SFGrid->EnableGridLines( true );
	SFGrid->EnableDragGridSize( false );
	SFGrid->SetMargins( 0, 0 );

	// Columns
	SFGrid->SetColSize( 0, 729 );
	SFGrid->EnableDragColMove( false );
	SFGrid->EnableDragColSize( true );
	SFGrid->SetColLabelValue( 0, wxT("File") );
	SFGrid->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	SFGrid->EnableDragRowSize( false );
	SFGrid->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	SFGrid->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	bSizer7->Add( SFGrid, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* BtnSizerSF;
	BtnSizerSF = new wxBoxSizer( wxHORIZONTAL );

	BtnLoad = new wxButton( SoundFontPanel, wxID_ANY, wxT("Load SoundFont in slot"), wxDefaultPosition, wxDefaultSize, 0 );
	BtnSizerSF->Add( BtnLoad, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	BtnUnLoad = new wxButton( SoundFontPanel, wxID_ANY, wxT("Unload SoundFont from slot"), wxDefaultPosition, wxDefaultSize, 0 );
	BtnSizerSF->Add( BtnUnLoad, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	BtnTestSFLib = new wxButton( SoundFontPanel, wxID_ANY, wxT("Test libsoundfont"), wxDefaultPosition, wxDefaultSize, 0 );
	BtnTestSFLib->Enable( false );
	BtnTestSFLib->Hide();

	BtnSizerSF->Add( BtnTestSFLib, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	bSizer7->Add( BtnSizerSF, 1, wxEXPAND, 5 );


	SoundFontPanel->SetSizer( bSizer7 );
	SoundFontPanel->Layout();
	bSizer7->Fit( SoundFontPanel );
	SFPresetNoteBook->AddPage( SoundFontPanel, wxT("SoundFonts"), true );
	PresetPanel = new wxPanel( SFPresetNoteBook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );

	PresetGrid = new wxGrid( PresetPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	PresetGrid->CreateGrid( 16, 3 );
	PresetGrid->EnableEditing( true );
	PresetGrid->EnableGridLines( true );
	PresetGrid->EnableDragGridSize( false );
	PresetGrid->SetMargins( 0, 0 );

	// Columns
	PresetGrid->SetColSize( 0, 377 );
	PresetGrid->SetColSize( 1, 84 );
	PresetGrid->SetColSize( 2, 277 );
	PresetGrid->EnableDragColMove( false );
	PresetGrid->EnableDragColSize( true );
	PresetGrid->SetColLabelValue( 0, wxT("SoundFont") );
	PresetGrid->SetColLabelValue( 1, wxT("Bank") );
	PresetGrid->SetColLabelValue( 2, wxT("Program") );
	PresetGrid->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	PresetGrid->EnableDragRowSize( false );
	PresetGrid->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	PresetGrid->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	bSizer9->Add( PresetGrid, 1, wxALL, 5 );

	wxBoxSizer* BtnSizerPreset;
	BtnSizerPreset = new wxBoxSizer( wxHORIZONTAL );


	bSizer9->Add( BtnSizerPreset, 1, wxEXPAND, 5 );


	PresetPanel->SetSizer( bSizer9 );
	PresetPanel->Layout();
	bSizer9->Fit( PresetPanel );
	SFPresetNoteBook->AddPage( PresetPanel, wxT("Presets"), false );

	TopSizer->Add( SFPresetNoteBook, 0, wxALL, 5 );


	this->SetSizer( TopSizer );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CSoundFontPresetManagerDialog::OnClose ) );
	SFPresetNoteBook->Connect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( CSoundFontPresetManagerDialog::OnNotebookPageChanged ), NULL, this );
	SFGrid->Connect( wxEVT_GRID_CELL_LEFT_DCLICK, wxGridEventHandler( CSoundFontPresetManagerDialog::OnSFGridDblClick ), NULL, this );
	SFGrid->Connect( wxEVT_GRID_SELECT_CELL, wxGridEventHandler( CSoundFontPresetManagerDialog::OnSFGridSelected ), NULL, this );
	BtnLoad->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSoundFontPresetManagerDialog::OnBtnLoad ), NULL, this );
	BtnUnLoad->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSoundFontPresetManagerDialog::OnBtnUnload ), NULL, this );
	BtnTestSFLib->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSoundFontPresetManagerDialog::OnBtnTestSFLib ), NULL, this );
	PresetGrid->Connect( wxEVT_GRID_CELL_CHANGED, wxGridEventHandler( CSoundFontPresetManagerDialog::OnPresetGridChange ), NULL, this );
	PresetGrid->Connect( wxEVT_GRID_SELECT_CELL, wxGridEventHandler( CSoundFontPresetManagerDialog::OnPresetGridSelected ), NULL, this );
}

CSoundFontPresetManagerDialog::~CSoundFontPresetManagerDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CSoundFontPresetManagerDialog::OnClose ) );
	SFPresetNoteBook->Disconnect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( CSoundFontPresetManagerDialog::OnNotebookPageChanged ), NULL, this );
	SFGrid->Disconnect( wxEVT_GRID_CELL_LEFT_DCLICK, wxGridEventHandler( CSoundFontPresetManagerDialog::OnSFGridDblClick ), NULL, this );
	SFGrid->Disconnect( wxEVT_GRID_SELECT_CELL, wxGridEventHandler( CSoundFontPresetManagerDialog::OnSFGridSelected ), NULL, this );
	BtnLoad->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSoundFontPresetManagerDialog::OnBtnLoad ), NULL, this );
	BtnUnLoad->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSoundFontPresetManagerDialog::OnBtnUnload ), NULL, this );
	BtnTestSFLib->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSoundFontPresetManagerDialog::OnBtnTestSFLib ), NULL, this );
	PresetGrid->Disconnect( wxEVT_GRID_CELL_CHANGED, wxGridEventHandler( CSoundFontPresetManagerDialog::OnPresetGridChange ), NULL, this );
	PresetGrid->Disconnect( wxEVT_GRID_SELECT_CELL, wxGridEventHandler( CSoundFontPresetManagerDialog::OnPresetGridSelected ), NULL, this );

}
