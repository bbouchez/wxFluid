/*
 *  SFPresetManager.h
 *  wxFluid Synthesizer
 *  wxFluid SoundFont and Presets management dialog
 *
 * Copyright (c) 2025 Benoit BOUCHEZ
 * License : MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef __SFPRESETMANAGER_H__
#define __SFPRESETMANAGER_H__

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wxFluidFontSelectorForm.h"
#include "SynthClass.h"

class CSoundFontPresetManager : public CSoundFontPresetManagerDialog
{
public:
	CSoundFontPresetManager(wxWindow* Parent, CwxFluidSynth* FluidSynth);

	void OnClose(wxCloseEvent& event);

	void OnBtnLoad(wxCommandEvent& event);
	void OnBtnUnload(wxCommandEvent& event);
	//void OnBtnDone(wxCommandEvent& event);
	
	void OnNotebookPageChanged(wxNotebookEvent& event);

	void OnSFGridSelected(wxGridEvent& event);
	void OnSFGridDblClick(wxGridEvent& event);

	void OnPresetGridSelected(wxGridEvent& event);
	void OnPresetGridChange(wxGridEvent& event);

private:
	CwxFluidSynth* Synth;
	int SelectedSFLine;
	int SelectedMIDIChannel;

	// Array to make list of soundfont appear as consecutive (they can be not consecutive in the synth memory)
	unsigned int TotalSFChoices;
	wxString SFNameChoice[MAX_SOUND_FONTS];			// Name without path
	int SFIndexChoice[MAX_SOUND_FONTS];				// Index of SoundFont 

	// List of presets for the currently selected channel
	wxString PresetName[128];		// List of concatenated presets (no spaces between names)
	//int PresetIndex[128];			// MIDI program number associated to the preset
	unsigned int TotalPresets;

	//! Fill internal array with soundfont names (all names are consecutive in the array)
	void RebuildSoundFontList(void);
	//! Display bank / preset / soundfont for the 16 channels
	void RefreshPresetGrid(void);
	//! Display bank / preset / soundfont for one MIDI channel
	void RefreshGridForChannel(unsigned int MIDIChannel);
	//! Display bank number in the MSB/LSB notation
	void DisplayBankNumber(unsigned int MIDIChannel);
	//! Fill preset list for the currenly selected channel
	void FillPresetsList(void);

	//! Open the Load SoundFont dialog and assign the SoundFont to selected slot
	void LoadSoundFontInSlot(void);
};

#endif
