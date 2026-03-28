/////////////////////////////////////////////////////////////////////////////
// SFPresetManager.cpp
// wxFluid SoundFont and Presets management
// (c) Benoit BOUCHEZ
/////////////////////////////////////////////////////////////////////////////

#include "SFPresetManager.h"
#include "libsoundfont.h"

// Columns index on Preset tab
#define SOUNDFONT_COLUMN		0
#define BANK_COLUMN				1
#define PRESET_COLUMN			2

extern bool ConfigChanged;

CSoundFontPresetManager::CSoundFontPresetManager(wxWindow* Parent, CwxFluidSynth* FluidSynth) : CSoundFontPresetManagerDialog(Parent)
{
	this->Synth = FluidSynth;
	this->SelectedSFLine = 0;			// wxGrid selects first line by default when displayed the first time

	for (int i = 0; i < MAX_SOUND_FONTS; i++)
	{
		this->SFGrid->SetReadOnly(i, 0, true);
		this->SFGrid->SetCellValue(i, 0, FluidSynth->SoundFontFilePath[i]);
	}

	this->SelectedMIDIChannel = -1;
#ifndef __TARGET_LINUX__
    // The SetCornerLabelValue is not available on wxGTK 3.0    Sigh....
	this->PresetGrid->SetCornerLabelValue("Channel");
	this->SFGrid->SetCornerLabelValue("Slot");
#endif // __TARGET_LINUX__

	this->TotalSFChoices = 0;
	for (int i = 0; i < MAX_SOUND_FONTS; i++)
	{
		this->SFNameChoice[i] = "";
		this->SFIndexChoice[i] = -1;
	}

	for (int i = 0; i < 16; i++)
	{
		PresetGrid->SetCellAlignment(i, BANK_COLUMN, wxALIGN_CENTRE, wxALIGN_CENTRE);	
	}

	this->RebuildSoundFontList();
	this->RefreshPresetGrid();

#ifdef __TARGET_LINUX__
	this->SetSize(800, 700);
	this->SFGrid->SetColSize(0, 800);
#endif
}  // CSoundFontPresetManager::CSoundFontPresetManager
// -----------------------------------------------------

void CSoundFontPresetManager::OnNotebookPageChanged(wxNotebookEvent& WXUNUSED(event))
{
	int TotalSoundfonts = 0;

	// If we go to preset page, we must have at least one SoundFont loaded
	if (this->SFPresetNoteBook->GetSelection() == 1)
	{
		// We must have at least one soundfont loaded before we can display this dialog
		for (int i = 0; i < MAX_SOUND_FONTS; i++)
		{
			if (this->Synth->FontID[i] != -1)
				TotalSoundfonts++;
		}

		if (TotalSoundfonts == 0)
		{
			wxMessageBox("At least one SoundFont must be loaded before a preset can be selected", "No SoundFont loaded", wxOK + wxICON_INFORMATION);
			this->SFPresetNoteBook->ChangeSelection(0);		// Go back to first page to load a soundfont
			return;
		}
	}
}  // CSoundFontPresetManager::OnNotebookPageChanged
// -----------------------------------------------------

void CSoundFontPresetManager::OnBtnLoad(wxCommandEvent& WXUNUSED(event))
{
	this->LoadSoundFontInSlot();
}  // CSoundFontPresetManager::OnBtnLoad
// -----------------------------------------------------

void CSoundFontPresetManager::OnBtnUnload(wxCommandEvent& WXUNUSED(event))
{
	if (this->SelectedSFLine == -1)
	{
		wxMessageBox("Select a SoundFont slot first", "Delete SoundFont", wxOK + wxICON_INFORMATION);
		return;
	}

	// TODO : add a confirmation message box before removing the soundfont
	ConfigChanged = true;
	this->Synth->PresetConfigurationChanged = true;
	Synth->UnloadSoundFont(SelectedSFLine);
	this->SFGrid->SetCellValue(this->SelectedSFLine, 0, "");

	this->RebuildSoundFontList();
	this->RefreshPresetGrid();
}  // CSoundFontPresetManager::OnBtnUnload
// -----------------------------------------------------

void CSoundFontPresetManager::OnBtnDone(wxCommandEvent& WXUNUSED(event))
{
	this->EndModal(wxID_OK);
}  // CSoundFontPresetManager::OnBtnDone
// -----------------------------------------------------

void CSoundFontPresetManager::OnSFGridSelected(wxGridEvent& event)
{
	int Row = event.GetRow();

	this->SelectedSFLine = Row;
}  // CSoundFontPresetManager::OnGridSelected
// -----------------------------------------------------

void CSoundFontPresetManager::OnSFGridDblClick(wxGridEvent& event)
{
	int Row = event.GetRow();

	this->SelectedSFLine = Row;
	this->LoadSoundFontInSlot();
}  // CSoundFontPresetManager::OnSFGridDblClick
// -----------------------------------------------------

void CSoundFontPresetManager::LoadSoundFontInSlot(void)
{
	int SFID;
	wxString FilePath;

	if (this->SelectedSFLine == -1)
	{
		wxMessageBox("Select a SoundFont slot first", "Load SoundFont", wxOK + wxICON_INFORMATION);
		return;
	}

	wxFileDialog dialog(this, wxT("Load SoundFont"), wxEmptyString, wxEmptyString, wxT("SF2 Soundfonts (*.sf2)|*.sf2;*.SF2"), wxFD_OPEN + wxFD_FILE_MUST_EXIST);
	if (dialog.ShowModal() == wxID_OK)
	{
		// If there was already a SoundFont loaded in this slot, unload it
		ConfigChanged = true;
		this->Synth->PresetConfigurationChanged = true;
		Synth->UnloadSoundFont(SelectedSFLine);
		this->SFGrid->SetCellValue(this->SelectedSFLine, 0, "");

		FilePath = dialog.GetPath();
		SFID = this->Synth->LoadSoundFont(FilePath, SelectedSFLine);
		if (SFID <= 0)
		{
			wxMessageBox("Can not load the SoundFont file", "Error", wxOK + wxICON_ERROR);
		}
		else
		{
			// Deactivate all channels using the soundfont, as loading a soundfont does not load any preset
			// This force the user to select a soundfont and a preset, so it is reloaded in the engine
			for (int Channel = 0; Channel < 16; Channel++)
			{
				if (this->Synth->SoundFontSlotForMIDIChannel[Channel] == this->SelectedSFLine)
				{
					this->Synth->SoundFontSlotForMIDIChannel[Channel] = -1;
				}
			}

			this->RebuildSoundFontList();
			this->RefreshPresetGrid();
			this->SFGrid->SetCellValue(this->SelectedSFLine, 0, FilePath);
		}
	}
}  // CSoundFontPresetManager::LoadSoundFontInSlot
// -----------------------------------------------------

void CSoundFontPresetManager::OnPresetGridSelected(wxGridEvent& event)
{
	int Col = event.GetCol();
	int Row = event.GetRow();

	if (Row < 0) return;
	this->SelectedMIDIChannel = Row;

	if (Col == BANK_COLUMN)
	{  // Nothing to do : enter manually the bank number

	}
	else if (Col == PRESET_COLUMN)
	{
		this->FillPresetsList();
		if (this->TotalPresets > 0)
		{
			this->PresetGrid->SetCellEditor(this->SelectedMIDIChannel, PRESET_COLUMN, new wxGridCellChoiceEditor(this->TotalPresets, this->PresetName, false));
			this->PresetGrid->ShowCellEditControl();
		}
		else
		{
			wxMessageBox("The soundfont does not contain presets for this bank", "Preset selection", wxOK + wxICON_INFORMATION);
		}
	}
	else if (Col == SOUNDFONT_COLUMN)
	{  // Fill Choices with list of available sound fonts
		if (this->TotalSFChoices > 0)
		{
			this->PresetGrid->SetCellEditor(this->SelectedMIDIChannel, SOUNDFONT_COLUMN, new wxGridCellChoiceEditor(this->TotalSFChoices, this->SFNameChoice, false));
			this->PresetGrid->ShowCellEditControl();
		}
	}
}  // CSoundFontPresetManager::OnPresetGridSelected
// -----------------------------------------------------

void CSoundFontPresetManager::OnPresetGridChange(wxGridEvent& event)
{
	int Col = event.GetCol();
	int Row = event.GetRow();
	wxString CellValue;
	wxString MSBString;
	wxString LSBString;
	int SFIndex;
	int BankTest;
	const char* name;
	int MSBTest, LSBTest;
	wxString PresetNumStr;

	if (Row < 0) return;
	this->SelectedMIDIChannel = Row;

	ConfigChanged = true;
	if (Col == BANK_COLUMN)
	{  // Bank is changed : format can be directly decimal from 0 to 16383 or MSB/LSB
		CellValue = this->PresetGrid->GetCellValue(Row, Col);
		if (CellValue == "")
		{
			this->Synth->PresetBank[SelectedMIDIChannel] = 0;
			this->DisplayBankNumber(SelectedMIDIChannel);
		}
		else if (CellValue.Find('/') > 0)
		{  // Separator found : format is MSB / LSB
			MSBString = CellValue.BeforeFirst('/');
			LSBString = CellValue.AfterFirst('/');

			MSBTest = wxAtoi(MSBString);
			LSBTest = wxAtoi(LSBString);
			if ((MSBTest < 0) || (MSBTest > 127) || (LSBTest < 0) || (LSBTest > 127))
			{
				this->DisplayBankNumber(this->SelectedMIDIChannel);
				wxMessageBox("Invalid bank number", "Error", wxOK + wxICON_EXCLAMATION);
				return;
			}
			this->Synth->PresetBank[this->SelectedMIDIChannel] = (MSBTest << 7) + LSBTest;
			this->RefreshGridForChannel(this->SelectedMIDIChannel);
		}
		else
		{
			BankTest = wxAtoi(CellValue);
			if ((BankTest < 0) || (BankTest > 16383))
			{
				this->DisplayBankNumber(this->SelectedMIDIChannel);
				wxMessageBox("Invalid bank number", "Error", wxOK + wxICON_EXCLAMATION);
				return;
			}
			this->Synth->PresetBank[this->SelectedMIDIChannel] = BankTest;
			this->RefreshGridForChannel(this->SelectedMIDIChannel);
		}

		// Retrieve program name within the new bank
		SFIndex = this->Synth->SoundFontSlotForMIDIChannel[this->SelectedMIDIChannel];
		fluid_sfont_t* sfont = fluid_synth_get_sfont_by_id(this->Synth->synth, this->Synth->FontID[SFIndex]);
		fluid_preset_t* preset = fluid_sfont_get_preset(sfont, this->Synth->PresetBank[this->SelectedMIDIChannel], this->Synth->ProgNumber[this->SelectedMIDIChannel]);

		// Load the new program in the synth
		if (preset == nullptr)
		{  // No preset available for this bank / preset number
			this->PresetGrid->SetCellValue(this->SelectedMIDIChannel, PRESET_COLUMN, "");		// Erase program name to show the issue
		}
		else
		{
			name = fluid_preset_get_name(preset);
			this->PresetGrid->SetCellValue(this->SelectedMIDIChannel, PRESET_COLUMN, name);
			fluid_synth_program_select(this->Synth->synth, this->SelectedMIDIChannel, this->Synth->FontID[SFIndex], this->Synth->PresetBank[this->SelectedMIDIChannel], this->Synth->ProgNumber[this->SelectedMIDIChannel]);
		}
	}
	else if (Col == PRESET_COLUMN)		// Preset is changed
	{  // It is not possible to get the index of the selected preset from the wxGridCellEditor
		// As the MIDI program number is included in the string, we extract the number from the name string and use it to know the preset number
		CellValue = this->PresetGrid->GetCellValue(Row, Col);
		PresetNumStr = CellValue.SubString(1, 3);
		this->Synth->ProgNumber[this->SelectedMIDIChannel] = wxAtoi(PresetNumStr) - 1;
		SFIndex = this->Synth->SoundFontSlotForMIDIChannel[this->SelectedMIDIChannel];
		fluid_synth_program_select(this->Synth->synth, this->SelectedMIDIChannel, this->Synth->FontID[SFIndex], this->Synth->PresetBank[this->SelectedMIDIChannel], this->Synth->ProgNumber[this->SelectedMIDIChannel]);
		return;
	}
	else if (Col == SOUNDFONT_COLUMN)
	{  // SoundFont is changed : clear preset name (or use the first preset by default ?)
		this->PresetGrid->SetCellValue(this->SelectedMIDIChannel, PRESET_COLUMN, "");

		// We can not get the soundfont index directly from the choices
		// We need to look for the font name
		CellValue = this->PresetGrid->GetCellValue(Row, Col);

		for (int i = 0; i < MAX_SOUND_FONTS; i++)
		{
			if (CellValue == SFNameChoice[i])
			{
				this->Synth->SoundFontSlotForMIDIChannel[this->SelectedMIDIChannel] = this->SFIndexChoice[i];
				return;
			}
		}
	}
}  // CSoundFontPresetManager::OnGridChange
// -----------------------------------------------------

void CSoundFontPresetManager::RefreshPresetGrid(void)
{
	for (int i = 0; i < 16; i++)
	{
		this->RefreshGridForChannel(i);
	}
}  // CSoundFontPresetManager::RefreshPresetGrid
// -----------------------------------------------------

void CSoundFontPresetManager::RebuildSoundFontList(void)
{
	wxFileName FileName;

	this->TotalSFChoices = 0;

	for (int i = 0; i < MAX_SOUND_FONTS; i++)
	{
		if (this->Synth->FontID[i] != -1)  // Only take loaded SoundFonts into account, as we will need it to retrieve the preset names
		{
			FileName = this->Synth->SoundFontFilePath[i];
			SFNameChoice[TotalSFChoices] = FileName.GetName();
			SFIndexChoice[TotalSFChoices] = i;
			this->TotalSFChoices += 1;
		}
	}
}  // CSoundFontPresetManager::RebuildSoundFontList
// -----------------------------------------------------

void CSoundFontPresetManager::RefreshGridForChannel(unsigned int MIDIChannel)
{
	int SFIndex;
	wxFileName FileName;
	fluid_sfont_t* sfont;
	fluid_preset_t* preset;
	const char* name;
	wxString Name;

	this->DisplayBankNumber(MIDIChannel);

	SFIndex = this->Synth->SoundFontSlotForMIDIChannel[MIDIChannel];
	if (SFIndex != -1)
	{
		if (this->Synth->FontID[SFIndex] != -1)
		{
			FileName = this->Synth->SoundFontFilePath[SFIndex];
			this->PresetGrid->SetCellValue(MIDIChannel, SOUNDFONT_COLUMN, FileName.GetName());

			sfont = fluid_synth_get_sfont_by_id(this->Synth->synth, this->Synth->FontID[SFIndex]);
			preset = fluid_sfont_get_preset(sfont, this->Synth->PresetBank[MIDIChannel], this->Synth->ProgNumber[MIDIChannel]);
			if (preset != nullptr)
			{
				name = fluid_preset_get_name(preset);
				Name = wxString::Format("(%03d) ", this->Synth->ProgNumber[MIDIChannel] + 1) + name;
				this->PresetGrid->SetCellValue(MIDIChannel, PRESET_COLUMN, Name);
			}
		}
		else
		{
			this->PresetGrid->SetCellValue(MIDIChannel, PRESET_COLUMN, "");
			this->PresetGrid->SetCellValue(MIDIChannel, SOUNDFONT_COLUMN, "");
		}
	}
	else
	{
		this->PresetGrid->SetCellValue(MIDIChannel, PRESET_COLUMN, "");
		this->PresetGrid->SetCellValue(MIDIChannel, SOUNDFONT_COLUMN, "");
	}
}  // CSoundFontPresetManager::RefreshGridForChannel
// -----------------------------------------------------

void CSoundFontPresetManager::DisplayBankNumber(unsigned int MIDIChannel)
{
	int BankLSB, BankMSB;

	BankLSB = this->Synth->PresetBank[MIDIChannel] & 0x7F;
	BankMSB = this->Synth->PresetBank[MIDIChannel] >> 7;
	this->PresetGrid->SetCellValue(MIDIChannel, BANK_COLUMN, wxString::Format("%d", BankMSB) + "/" + wxString::Format("%d", BankLSB));
}  // CSoundFontPresetManager::DisplayBankNumber
// -----------------------------------------------------

void CSoundFontPresetManager::FillPresetsList(void)
{
	int SFIndex;
	int PresetNumber;
	wxString Name;

	this->TotalPresets = 0;

	if (this->SelectedMIDIChannel == -1) return;

	// If there is no soundfont loaded for the channel or if soundfont was not loaded, no preset can be displayed
	SFIndex = this->Synth->SoundFontSlotForMIDIChannel[this->SelectedMIDIChannel];
	if (SFIndex == -1) return;		// No sound font selected for this channel : can not create a preset list
	if (this->Synth->FontID[SFIndex] == -1) return;

	// Fill preset list
	fluid_sfont_t* sfont = fluid_synth_get_sfont_by_id(this->Synth->synth, this->Synth->FontID[SFIndex]);
	for (PresetNumber = 0; PresetNumber < 128; PresetNumber++)
	{
		fluid_preset_t* preset = fluid_sfont_get_preset(sfont, this->Synth->PresetBank[this->SelectedMIDIChannel], PresetNumber);
		if (preset != nullptr)
		{
			// Add the preset number in front of the name to provide the MIDI program number to use
			const char* name = fluid_preset_get_name(preset);

			Name = wxString::Format("(%03d) ", PresetNumber + 1) + name;
			this->PresetName[this->TotalPresets] = Name;
			//this->PresetIndex[this->TotalPresets] = PresetNumber;
			this->TotalPresets += 1;
		}
	}
}  // CSoundFontPresetManager::FillPresetsList
// -----------------------------------------------------

