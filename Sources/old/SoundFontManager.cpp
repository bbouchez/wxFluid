/////////////////////////////////////////////////////////////////////////////
// SoundFontManager.cpp
// wxFluid SoundFont management (load / unload)
// (c) Benoit BOUCHEZ
/////////////////////////////////////////////////////////////////////////////

#include "SoundFontManager.h"

CSoundFontManager::CSoundFontManager(wxWindow* Parent, CwxFluidSynth* FluidSynth) : CSoundFontManagerDialog(Parent)
{
	this->Synth = FluidSynth;
	this->SelectedLine = 0;			// wxGrid selects first line by default when displayed the first time

	for (int i = 0; i < MAX_SOUND_FONTS; i++)
	{
		this->SFGrid->SetReadOnly(i, 0, true);
		this->SFGrid->SetCellValue(i, 0, FluidSynth->SoundFontFilePath[i]);
	}

#ifdef __TARGET_LINUX__
	this->SetSize(800, 600);
	this->SFGrid->SetColSize(0, 800);
#endif
}  // CSoundFontManager::CSoundFontManager
// -----------------------------------------------------

void CSoundFontManager::OnBtnLoad(wxCommandEvent& WXUNUSED(event))
{
	int SFID;
	wxString FilePath;

	if (this->SelectedLine == -1)
	{
		wxMessageBox("Select a SoundFont slot first", "Load SoundFont", wxOK + wxICON_INFORMATION);
		return;
	}

	wxFileDialog dialog(this, wxT("Load SoundFont"), wxEmptyString, wxEmptyString, wxT("SF2 Soundfonts (*.sf2)|*.sf2;*.SF2"), wxFD_OPEN + wxFD_FILE_MUST_EXIST);
	if (dialog.ShowModal() == wxID_OK)
	{
		this->Synth->PresetConfigurationChanged = true;

		FilePath = dialog.GetPath();
		SFID = this->Synth->LoadSoundFont(FilePath, SelectedLine);
		if (SFID > 0)
		{  // SoundFont loaded correctly
			this->SFGrid->SetCellValue(this->SelectedLine, 0, dialog.GetPath());
		}
		else
		{
			switch (SFID)
			{
			case -2:
				wxMessageBox("Unload SoundFont before loading a new one in slot", "Error", wxOK + wxICON_INFORMATION);
				break;
			default:
				wxMessageBox("Can not load the SoundFont file", "Error", wxOK + wxICON_ERROR);
			}
		}
	}
}  // CSoundFontManager::OnBtnLoad
// -----------------------------------------------------

void CSoundFontManager::OnBtnUnload(wxCommandEvent& WXUNUSED(event))
{
	if (this->SelectedLine == -1)
	{
		wxMessageBox("Select a SoundFont slot first", "Delete SoundFont", wxOK + wxICON_INFORMATION);
		return;
	}

	// TODO : add a confirmation message box before removing the soundfont
	this->Synth->PresetConfigurationChanged = true;
	Synth->UnloadSoundFont(SelectedLine);
	this->SFGrid->SetCellValue(this->SelectedLine, 0, "");
}  // CSoundFontManager::OnBtnUnload
// -----------------------------------------------------

void CSoundFontManager::OnGridSelected(wxGridEvent& event)
{
	int Row = event.GetRow();

	this->SelectedLine = Row;
}  // CSoundFontManager::OnGridSelected
// -----------------------------------------------------

