/////////////////////////////////////////////////////////////////////////////
// PresetSelection.h
// wxFluid preset selector for each MIDI channel
// (c) Benoit BOUCHEZ
/////////////////////////////////////////////////////////////////////////////

#ifndef __CHANNELPRESETFRAME_H__
#define __CHANNELPRESETFRAME_H__

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wxFluidFontSelectorForm.h"
#include "SynthClass.h"

class CPresetSelector : public CPresetSelectDialog
{
public:
	CPresetSelector(wxWindow* Parent, CwxFluidSynth* FluidSynth);

	void OnGridSelected(wxGridEvent& event);
	void OnGridChange(wxGridEvent& event);
	
private:
	CwxFluidSynth* Synth;
	int SelectedMIDIChannel;

	// Array to make list of soundfont appear as consecutive (they can be not consecutive in the synth memory)
	unsigned int TotalSFChoices;
	wxString SFNameChoice[MAX_SOUND_FONTS];			// Name without path
	int SFIndexChoice[MAX_SOUND_FONTS];				// Index of SoundFont 

	// List of presets for the currently selected channel
	wxString PresetName[128];		// List of concatenated presets (no spaces between names)
	//int PresetIndex[128];			// MIDI program number associated to the preset
	unsigned int TotalPresets;

	//! Fill preset list for the currenly selected channel
	void FillPresetsList(void);

	//! Fill internal array with soundfont names (all names are consecutive in the array)
	void BuildSoundFontList(void);

	//! Display bank number in the MSB/LSB notation
	void DisplayBankNumber(unsigned int MIDIChannel);
	//! Display bank / preset / soundfont for one MIDI channel
	void RefreshGridForChannel(unsigned int MIDIChannel);
	//! Display bank / preset / soundfont for the 16 channels
	void RefreshPresetGrid(void);

};

#endif
