/////////////////////////////////////////////////////////////////////////////
// PresetSelection.cpp
// wxFluid preset selector for each MIDI channel
// (c) Benoit BOUCHEZ
/////////////////////////////////////////////////////////////////////////////

#include "PresetSelection.h"

CPresetSelector::CPresetSelector(wxWindow* Parent, CwxFluidSynth* FluidSynth) : CPresetSelectDialog(Parent)
{
	this->Synth = FluidSynth;
	this->SelectedMIDIChannel = -1;
	this->PresetGrid->HideRowLabels();

	this->TotalSFChoices = 0;
	for (int i = 0; i < MAX_SOUND_FONTS; i++)
	{
		this->SFNameChoice[i] = "";
		this->SFIndexChoice[i] = -1;
	}

	for (int i = 0; i < 16; i++)
	{
		PresetGrid->SetCellAlignment(i, 0, wxALIGN_CENTRE, wxALIGN_CENTRE);
		PresetGrid->SetCellValue(i, 0, wxString::Format("%d", i + 1));

		PresetGrid->SetCellAlignment(i, 1, wxALIGN_CENTRE, wxALIGN_CENTRE);

		PresetGrid->SetReadOnly(i, 0, true);
	}

	this->BuildSoundFontList();
	this->RefreshPresetGrid();
}  // CPresetSelector::CPresetSelector
// -----------------------------------------------------

void CPresetSelector::OnGridSelected(wxGridEvent& event)
{
	int Col = event.GetCol();
	int Row = event.GetRow();

	if (Row < 0) return;
	this->SelectedMIDIChannel = Row;

	if (Col == 1)
	{  // Nothing to do : enter manually the bank number

	}
	else if (Col == 2)
	{
		this->FillPresetsList();
		if (this->TotalPresets > 0)
		{
			this->PresetGrid->SetCellEditor(this->SelectedMIDIChannel, 2, new wxGridCellChoiceEditor(this->TotalPresets, this->PresetName, false));
			this->PresetGrid->ShowCellEditControl();
		}
		else
		{
			wxMessageBox("The soundfont does not contain presets for this bank", "Preset selection", wxOK + wxICON_INFORMATION);
		}
	}
	else if (Col == 3)
	{  // Fill Choices with list of available sound fonts
		if (this->TotalSFChoices > 0)
		{
			this->PresetGrid->SetCellEditor(this->SelectedMIDIChannel, 3, new wxGridCellChoiceEditor(this->TotalSFChoices, this->SFNameChoice, false));
			this->PresetGrid->ShowCellEditControl();
		}
	}
}  // CPresetSelector::OnGridSelected
// -----------------------------------------------------

void CPresetSelector::OnGridChange(wxGridEvent& event)
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

	if (Col == 1)
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
			this->Synth->PresetBank[this->SelectedMIDIChannel] = (MSBTest<<7)+LSBTest;
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
		SFIndex = this->Synth->SoundFontIndexForChannel[this->SelectedMIDIChannel];
		fluid_sfont_t* sfont = fluid_synth_get_sfont_by_id(this->Synth->synth, this->Synth->FontID[SFIndex]);
		fluid_preset_t* preset = fluid_sfont_get_preset(sfont, this->Synth->PresetBank[this->SelectedMIDIChannel], this->Synth->ProgNumber[this->SelectedMIDIChannel]);

		// Load the new program in the synth
		if (preset == nullptr)
		{  // No preset available for this bank / preset number
			this->PresetGrid->SetCellValue(this->SelectedMIDIChannel, 2, "");		// Erase program name to show the issue
		}
		else
		{
			name = fluid_preset_get_name(preset);
			this->PresetGrid->SetCellValue(this->SelectedMIDIChannel, 2, name);
			fluid_synth_program_select(this->Synth->synth, this->SelectedMIDIChannel, this->Synth->FontID[SFIndex], this->Synth->PresetBank[this->SelectedMIDIChannel], this->Synth->ProgNumber[this->SelectedMIDIChannel]);
		}
	}
	else if (Col == 2)		// Preset is changed
	{  // It is not possible to get the index of the selected preset from the wxGridCellEditor
		// As the MIDI program number is included in the string, we extract the number from the name string and use it to know the preset number
		CellValue = this->PresetGrid->GetCellValue(Row, Col);
		PresetNumStr = CellValue.SubString(1, 3);
		this->Synth->ProgNumber[this->SelectedMIDIChannel] = wxAtoi(PresetNumStr)-1;
		SFIndex = this->Synth->SoundFontIndexForChannel[this->SelectedMIDIChannel];
		fluid_synth_program_select(this->Synth->synth, this->SelectedMIDIChannel, this->Synth->FontID[SFIndex], this->Synth->PresetBank[this->SelectedMIDIChannel], this->Synth->ProgNumber[this->SelectedMIDIChannel]);
		return;

		// Old method
		// Search for the string in the cell in the choices (does not work anymore with the preset number in the string)
		/*


		for (int i = 0; i < 128; i++)
		{
			if (CellValue == PresetName[i])
			{
				this->Synth->ProgNumber[this->SelectedMIDIChannel] = this->PresetIndex[i];
				SFIndex = this->Synth->SoundFontIndexForChannel[this->SelectedMIDIChannel];
				fluid_synth_program_select(this->Synth->synth, this->SelectedMIDIChannel, this->Synth->FontID[SFIndex], this->Synth->PresetBank[this->SelectedMIDIChannel], this->Synth->ProgNumber[this->SelectedMIDIChannel]);
				return;
			}
		}
		*/
	}
	else if (Col == 3)
	{  // SoundFont is changed : clear preset name (or use the first preset by default ?)
		this->PresetGrid->SetCellValue(this->SelectedMIDIChannel, 2, "");
		
		// We can not get the soundfont index directly from the choices
		// We need to look for the font name
		CellValue = this->PresetGrid->GetCellValue(Row, Col);

		for (int i = 0; i < MAX_SOUND_FONTS; i++)
		{
			if (CellValue == SFNameChoice[i])
			{
				this->Synth->SoundFontIndexForChannel[this->SelectedMIDIChannel] = this->SFIndexChoice[i];
				return;
			}
		}
	}
}  // CPresetSelector::OnGridChange
// -----------------------------------------------------

void CPresetSelector::BuildSoundFontList(void)
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
}  // CPresetSelector::BuildSoundFontList
// -----------------------------------------------------

void CPresetSelector::FillPresetsList(void)
{
	int SFIndex;
	int PresetNumber;
	wxString Name;

	this->TotalPresets = 0;

	if (this->SelectedMIDIChannel == -1) return;

	// If there is no soundfont loaded for the channel or if soundfont was not loaded, no preset can be displayed
	SFIndex = this->Synth->SoundFontIndexForChannel[this->SelectedMIDIChannel];
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

			Name = wxString::Format("(%03d) ", PresetNumber+1) + name;
			this->PresetName[this->TotalPresets] = Name;
			//this->PresetIndex[this->TotalPresets] = PresetNumber;
			this->TotalPresets += 1;
		}
	}
}  // CPresetSelector::FillPresetsList
// -----------------------------------------------------

void CPresetSelector::RefreshPresetGrid(void)
{
	for (int i = 0; i < 16; i++)
	{
		this->RefreshGridForChannel(i);
	}
}  // CPresetSelector::RefreshPresetGrid
// -----------------------------------------------------

void CPresetSelector::RefreshGridForChannel(unsigned int MIDIChannel)
{
	int SFIndex;
	wxFileName FileName;
	fluid_sfont_t* sfont;
	fluid_preset_t* preset;
	const char* name;
	wxString Name;

	this->DisplayBankNumber(MIDIChannel);

	this->PresetGrid->SetCellValue(MIDIChannel, 3, "");
	SFIndex = this->Synth->SoundFontIndexForChannel[MIDIChannel];
	if (SFIndex != -1)
	{
		if (this->Synth->FontID[SFIndex] != -1)
		{
			FileName = this->Synth->SoundFontFilePath[SFIndex];
			this->PresetGrid->SetCellValue(MIDIChannel, 3, FileName.GetName());

			sfont = fluid_synth_get_sfont_by_id(this->Synth->synth, this->Synth->FontID[SFIndex]);
			preset = fluid_sfont_get_preset(sfont, this->Synth->PresetBank[MIDIChannel], this->Synth->ProgNumber[MIDIChannel]);
			if (preset != nullptr)
			{
				name = fluid_preset_get_name(preset);
				Name = wxString::Format("(%03d) ", this->Synth->ProgNumber[MIDIChannel] + 1) + name;
				this->PresetGrid->SetCellValue(MIDIChannel, 2, Name);
			}
		}
	}
}  // CPresetSelector::RefreshGridForChannel
// -----------------------------------------------------

void CPresetSelector::DisplayBankNumber(unsigned int MIDIChannel)
{
	int BankLSB, BankMSB;

	BankLSB = this->Synth->PresetBank[MIDIChannel] & 0x7F;
	BankMSB = this->Synth->PresetBank[MIDIChannel] >> 7;
	this->PresetGrid->SetCellValue(MIDIChannel, 1, wxString::Format("%d", BankMSB) + "/" + wxString::Format("%d", BankLSB));
}  // CPresetSelector::DisplayBankNumber
// -----------------------------------------------------

