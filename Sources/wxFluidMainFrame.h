/////////////////////////////////////////////////////////////////////////////
// Name:        wxFluidMainFrame.h
// Purpose:     wxFluid Main frame
// Author:      Benoit BOUCHEZ
// Copyright:   (c) Benoit BOUCHEZ
/////////////////////////////////////////////////////////////////////////////

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
