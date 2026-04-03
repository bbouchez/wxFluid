/*
 *  wxFluidMainFrame.h
 *  wxFluid Synthesizer
 *  wxFluid main frame class
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

#ifndef __MAINFRAME_H__
#define __MAINFRAME_H__

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wxFluidAppConfig.h"
#include "wxFluidMainForms.h"
#include "VirtualKeyboard.h"
#include "wxAnimKnob.h"
#include "wxOnOffBitmapButton.h"
#include "KnobControllers.h"

extern wxBitmap* KnobBitmap;
extern wxBitmap* ToggleBitmapOff;
extern wxBitmap* ToggleBitmapOn;
extern wxBitmap* WaveBitmapTri;
extern wxBitmap* WaveBitmapSine;
extern wxBitmap* IconBitmap;

class wxFluidMainFrame : public CMainFrame
{
public:
	wxFluidMainFrame (wxWindow* parent);

    // event handlers (these functions should _not_ be virtual)
	void OnClose(wxCloseEvent &event);
	void OnRefreshTimer(wxTimerEvent& event);

    void OnQuit(wxCommandEvent& event);

	void OnMenuClearPresets(wxCommandEvent& event);
	void OnMenuLoadPresets(wxCommandEvent& event);
	void OnMenuSavePresets(wxCommandEvent& event);
	void OnMenuSavePresetsAs(wxCommandEvent& event);

	void OnMenuConfigureSynth(wxCommandEvent& event);
	void OnMenuConfigureMIDI(wxCommandEvent& event);

	void OnViewVirtualKeyboard(wxCommandEvent& event);
	void OnViewMIDIControls(wxCommandEvent& event);
	void OnConfigureSoundFontsPresets(wxCommandEvent& event);
	void OnViewPresets(wxCommandEvent& event);

	void OnAbout(wxCommandEvent& event);

	//! Refreshes all front panel controls with values in synth engine
	void UpdateFrontPanel(void);

private:
    wxDECLARE_EVENT_TABLE();

	CVirtualKeyboard* VirtualKeyboard;
	//CMIDIControllersFrame* MIDIControllers;
	wxString PresetFileName;

	wxAnimKnob* VolumeKnob;

	wxOnOffBitmapButton* ReverbOnToggle;
	wxAnimKnob* ReverbRoomSizeKnob;
	wxAnimKnob* ReverbDampingKnob;
	wxAnimKnob* ReverbWidthKnob;
	wxAnimKnob* ReverbLevelKnob;

	wxOnOffBitmapButton* ChorusOnToggle;
	wxAnimKnob* ChorusNKnob;
	wxAnimKnob* ChorusLevelKnob;
	wxAnimKnob* ChorusSpeedKnob;
	wxAnimKnob* ChorusDepthKnob;
	wxOnOffBitmapButton* ChorusWaveToggle;

	void OnVolumeKnob(wxCommandEvent& event);

	void OnReverbOnOff(wxCommandEvent& event);
	void OnReverbRoomSizeKnob(wxCommandEvent& event);
	void OnReverbDampingKnob(wxCommandEvent& event);
	void OnReverbWidthKnob(wxCommandEvent& event);
	void OnReverbLevelKnob(wxCommandEvent& event);

	void OnChorusOnOff(wxCommandEvent& event);
	void OnChorusNKnob(wxCommandEvent& event);
	void OnChorusLevelKnob(wxCommandEvent& event);
	void OnChorusSpeedKnob(wxCommandEvent& event);
	void OnChorusDepthKnob(wxCommandEvent& event);
	void OnChorusWave(wxCommandEvent& event);
};

#endif
