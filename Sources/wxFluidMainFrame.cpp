/////////////////////////////////////////////////////////////////////////////
// wxFluidMainFrame.cpp
// wxFluid Main frame
// (c) 2024 - Benoit BOUCHEZ (BEB)
/////////////////////////////////////////////////////////////////////////////

#include "wxFluidMainFrame.h"
#include "wxFluidApp.h"
#include "fluidsynth.h"
#ifdef __TARGET_WIN__
#include "rtaudio.h"
#endif
#include "SynthConfigurationDialog.h"
#include "MIDIConfigurationDialog.h"
#include "SynthClass.h"
#include "SFPresetManager.h"

// the application icon (under Windows it is in resources and even
// though we could still include the XPM here it would be unused)
#ifdef __TARGET_LINUX__
#ifndef wxHAS_IMAGES_IN_RESOURCES
//    #include "../Images/wxfluid_icon.xpm"
#endif
#endif

#define ID_VOLUME_KNOB			1000
#define ID_REVERB_ROOM_KNOB		1001
#define ID_REVERB_DAMP_KNOB		1002
#define ID_REVERB_WIDTH_KNOB	1003
#define ID_REVERB_LEVEL_KNOB	1005
#define ID_CHORUS_N_KNOB		1006
#define ID_CHORUS_LEVEL_KNOB	1007
#define ID_CHORUS_SPEED_KNOB	1008
#define ID_CHORUS_DEPTH_KNOB	1009
#define ID_REVERB_ON_OFF		1010
#define ID_CHORUS_ON_OFF		1011
#define ID_CHORUS_WAVE			1012

// On Linux and MacOS, the front panel controls can not have the background bitmap as parent
// The control position is then given relative to main window, so we have to offset them
#ifdef __TARGET_WIN__
#define OffsetXPanel		0
#define OffsetYPanel		0
#else
#define OffsetXPanel		15
#define OffsetYPanel		5
#endif

//! Global bitmaps
wxBitmap* KnobBitmap;
wxBitmap* ToggleBitmapOff;
wxBitmap* ToggleBitmapOn;
wxBitmap* WaveBitmapTri;
wxBitmap* WaveBitmapSine;
wxBitmap* IconBitmap;

bool ConfigChanged;

wxBEGIN_EVENT_TABLE(wxFluidMainFrame, wxFrame)
EVT_KNOB_CHANGE(ID_VOLUME_KNOB, wxFluidMainFrame::OnVolumeKnob)
EVT_KNOB_CHANGE(ID_REVERB_ROOM_KNOB, wxFluidMainFrame::OnReverbRoomSizeKnob)
EVT_KNOB_CHANGE(ID_REVERB_DAMP_KNOB, wxFluidMainFrame::OnReverbDampingKnob)
EVT_KNOB_CHANGE(ID_REVERB_WIDTH_KNOB, wxFluidMainFrame::OnReverbDampingKnob)
EVT_KNOB_CHANGE(ID_REVERB_LEVEL_KNOB, wxFluidMainFrame::OnReverbLevelKnob)
EVT_KNOB_CHANGE(ID_CHORUS_N_KNOB, wxFluidMainFrame::OnChorusNKnob)
EVT_KNOB_CHANGE(ID_CHORUS_LEVEL_KNOB, wxFluidMainFrame::OnChorusLevelKnob)
EVT_KNOB_CHANGE(ID_CHORUS_SPEED_KNOB, wxFluidMainFrame::OnChorusSpeedKnob)
EVT_KNOB_CHANGE(ID_CHORUS_DEPTH_KNOB, wxFluidMainFrame::OnChorusDepthKnob)
EVT_BUTTON(ID_REVERB_ON_OFF, wxFluidMainFrame::OnReverbOnOff)
EVT_BUTTON(ID_CHORUS_ON_OFF, wxFluidMainFrame::OnChorusOnOff)
EVT_BUTTON(ID_CHORUS_WAVE, wxFluidMainFrame::OnChorusWave)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(wxFluidApp);

CwxFluidSynth* GSynth;

wxFluidMainFrame::wxFluidMainFrame (wxWindow* parent) : CMainFrame (parent)
{
	wxWindow* ControlParent;
	FILE* LastPresetFile;
	unsigned char FileNameBuffer[1024];
	wxFileName FileName;
	size_t ReadSize;

#ifdef __TARGET_LINUX__
//	SetIcon(wxICON(wxfluid_icon));
#endif

	this->PresetFileName = "";
	ConfigChanged = false;
	GSynth = new CwxFluidSynth();
	if (GSynth == 0)
	{
		wxMessageBox("Can not create wxFluid engine", "Fatal error", wxOK + wxICON_STOP);
		this->Destroy();
		return;
	}

    GSynth->LoadEngineConfiguration();

    GSynth->CreateFluidEngine();            // Must be done after loading engine configuration as it uses the configuration parameters
    if (GSynth->synth == 0)
    {
        wxMessageBox("Can not create synthesis engine", "Fatal error", wxOK + wxICON_ERROR);
        // Question : is it safe to continue running wxFluid with synth not created ?
    }

#ifdef __TARGET_MAC__
    this->BackgroundBitmap->SetBitmap(wxBitmap("background", wxBITMAP_TYPE_PNG_RESOURCE));
#endif
#ifdef __TARGET_WIN__
	this->BackgroundBitmap->SetBitmap(wxBitmap("background", wxBITMAP_TYPE_PNG_RESOURCE));
#endif
#ifdef __TARGET_LINUX__
	this->BackgroundBitmap->SetBitmap(wxBitmap("images/background.png", wxBITMAP_TYPE_PNG));
#endif

	// On Linux and Mac OS platform, a wxBitmap can not be used as a parent for the controls
#ifdef __TARGET_WIN__
    ControlParent = this->BackgroundBitmap;
#else
    ControlParent = this;
#endif

	this->VolumeKnob = new wxAnimKnob(ControlParent, ID_VOLUME_KNOB, KnobBitmap, 66, 128, wxPoint(21+OffsetXPanel, 61+OffsetYPanel), wxSize(66, 66), wxBORDER_NONE);

	this->ReverbOnToggle = new wxOnOffBitmapButton(ControlParent, ID_REVERB_ON_OFF, ToggleBitmapOff, ToggleBitmapOn, wxPoint(131 + OffsetXPanel, 61 + OffsetYPanel), wxSize(60, 60), wxBORDER_NONE);
	this->ReverbRoomSizeKnob = new wxAnimKnob(ControlParent, ID_REVERB_ROOM_KNOB, KnobBitmap, 66, 128, wxPoint(201 + OffsetXPanel, 61 + OffsetYPanel), wxSize(66, 66), wxBORDER_NONE);
	this->ReverbDampingKnob = new wxAnimKnob(ControlParent, ID_REVERB_DAMP_KNOB, KnobBitmap, 66, 128, wxPoint(281 + OffsetXPanel, 61 + OffsetYPanel), wxSize(66, 66), wxBORDER_NONE);
	this->ReverbWidthKnob = new wxAnimKnob(ControlParent, ID_REVERB_WIDTH_KNOB, KnobBitmap, 66, 128, wxPoint(361 + OffsetXPanel, 61 + OffsetYPanel), wxSize(66, 66), wxBORDER_NONE);
	this->ReverbLevelKnob = new wxAnimKnob(ControlParent, ID_REVERB_LEVEL_KNOB, KnobBitmap, 66, 128, wxPoint(441 + OffsetXPanel, 61 + OffsetYPanel), wxSize(66, 66), wxBORDER_NONE);

	this->ChorusOnToggle = new wxOnOffBitmapButton(ControlParent, ID_CHORUS_ON_OFF, ToggleBitmapOff, ToggleBitmapOn, wxPoint(551 + OffsetXPanel, 61 + OffsetYPanel), wxSize(60, 60), wxBORDER_NONE);
	this->ChorusNKnob = new wxAnimKnob(ControlParent, ID_CHORUS_N_KNOB, KnobBitmap, 66, 128, wxPoint(621 + OffsetXPanel, 61 + OffsetYPanel), wxSize(66, 66), wxBORDER_NONE);
	this->ChorusSpeedKnob = new wxAnimKnob(ControlParent, ID_CHORUS_SPEED_KNOB, KnobBitmap, 66, 128, wxPoint(701 + OffsetXPanel, 61 + OffsetYPanel), wxSize(66, 66), wxBORDER_NONE);
	this->ChorusDepthKnob = new wxAnimKnob(ControlParent, ID_CHORUS_DEPTH_KNOB, KnobBitmap, 66, 128, wxPoint(781 + OffsetXPanel, 61 + OffsetYPanel), wxSize(66, 66), wxBORDER_NONE);
	this->ChorusLevelKnob = new wxAnimKnob(ControlParent, ID_CHORUS_LEVEL_KNOB, KnobBitmap, 66, 128, wxPoint(861 + OffsetXPanel, 61 + OffsetYPanel), wxSize(66, 66), wxBORDER_NONE);
	this->ChorusWaveToggle = new wxOnOffBitmapButton(ControlParent, ID_CHORUS_WAVE, WaveBitmapSine, WaveBitmapTri, wxPoint(740 + OffsetXPanel, 130 + OffsetYPanel), wxSize(66, 29), wxBORDER_NONE);

	this->VirtualKeyboard = new CVirtualKeyboard(this, GSynth);

	// Check if we find the file with last preset name
	if (wxFileExists("lastpreset.dat"))
	{
		LastPresetFile = fopen("lastpreset.dat", "rt");
		if (LastPresetFile != 0)
		{  // The file contains the full path to the wxFluid preset file
			ReadSize = fread(&FileNameBuffer[0], 1, 1024, LastPresetFile);
			if (ReadSize > 0)
			{
				if (ReadSize >= 1024)
					ReadSize = 1023;
				FileNameBuffer[ReadSize] = 0;		// Force the string to be terminated, just in case the file is corrupted
				this->PresetFileName = FileNameBuffer;

				GSynth->LoadPresetFile(this->PresetFileName);
				FileName = this->PresetFileName;
				this->SetTitle("wxFluid - " + FileName.GetName());
			}

			fclose(LastPresetFile);
		}
	}

	this->UpdateFrontPanel();		// Do it even if there is no preset file to load, to show default values

#ifdef __TARGET_LINUX__
	this->SetSize(980, 320);
#endif // __TARGET_LINUX__
#ifdef __TARGET_MAC__
    this->SetClientSize(980, 215);
#endif

    RefreshTimer.Start(100);
}  // wxFluidMainFrame::wxFluidPMainFrame
// -----------------------------------------------------

// Quit command by menu (not button on top bar)
void wxFluidMainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}  // wxFluidMainFrame::OnQuit
// -----------------------------------------------------

// Click on close icon on top bar or request to close the window sent by OnQuit
void wxFluidMainFrame::OnClose(wxCloseEvent& event)
{
	if (ConfigChanged)
	{
		if (wxMessageBox("Current preset configuration is not saved.\nAre you sure you want to quit ?", "Closing wxFluid", wxYES_NO + wxNO_DEFAULT + wxICON_WARNING) != wxYES)
		{
			event.Veto(true);
			return;
		}
	}

	delete VirtualKeyboard;
	VirtualKeyboard = 0;

	//delete this->MIDIControllers;
	//this->MIDIControllers = 0;

	delete GSynth;
	GSynth = 0;

	RefreshTimer.Stop();
	this->Destroy();
}  // wxFluidMainFrame::OnClose
// -----------------------------------------------------

void wxFluidMainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    AboutDialog* Dialog = 0;
	wxString AppVersionStr;
	wxString FluidSynthVersionStr;
	int major, minor, micro;

	Dialog = new AboutDialog(this);

	if (Dialog)
	{
		Dialog->ImageLogo->SetBitmap(*IconBitmap);
		AppVersionStr = "Version "+wxString::Format("%d", WXFLUID_VERSION) + "." + wxString::Format("%d", WXFLUID_REVISION) + "." + wxString::Format("%d", WXFLUID_SUBREVISION);
		Dialog->AppVersion->SetLabel (AppVersionStr);
		fluid_version(&major, &minor, &micro);
		FluidSynthVersionStr = "Using FluidSynth engine V" + wxString::Format("%d", major) + "." + wxString::Format("%d", minor) + "." + wxString::Format("%d", micro);
		Dialog->FluidSynthVersion->SetLabel(FluidSynthVersionStr);
		Dialog->ShowModal();
		delete Dialog;
	}
}  // wxFluidMainFrame::OnAbout
// -----------------------------------------------------

void wxFluidMainFrame::OnRefreshTimer(wxTimerEvent& WXUNUSED(event))
{
    int SessionStatus;
    bool SessionHasClosed;

    // Display connection status for RTP-MIDI / Network UMP
    if (GSynth->Config.NetworkMIDIMode == MIDI_NETWORK_RTP)
    {
        if (GSynth->RTPMIDIHandler)
        {
            SessionStatus = GSynth->RTPMIDIHandler->getSessionStatus();
            SessionHasClosed = GSynth->RTPMIDIHandler->RemotePeerHasRefusedSession();
            switch (SessionStatus)
            {
                case 0 :
                    this->StatusBar->SetStatusText("RTP-MIDI : device disconnected");
                    break;
                case 1 :
                    this->StatusBar->SetStatusText("RTP-MIDI : inviting remote device...");
                    if (SessionHasClosed)
                    {
                        this->StatusBar->SetStatusText("RTP-MIDI : remote device closed connection or refuse it...");
                    }
                    break;
                case 2 :
                    this->StatusBar->SetStatusText("RTP-MIDI : waiting for remote connection");
                    break;
                case 3 :
                    this->StatusBar->SetStatusText("RTP-MIDI : connected");
                    break;
                default :
                    this->StatusBar->SetStatusText("RTP-MIDI has an unknown status !");
            }
        }
    }
    else if (GSynth->Config.NetworkMIDIMode == MIDI_NETWORK_UMP)
    {
        if (GSynth->NetUMPHandler)
        {
            SessionStatus = GSynth->NetUMPHandler->GetSessionStatus();
            SessionHasClosed = GSynth->NetUMPHandler->RemotePeerClosedSession();
            switch (SessionStatus)
            {
                case 0 :
                    this->StatusBar->SetStatusText("Network UMP : device disconnected");
                    break;
                case 1 :
                    this->StatusBar->SetStatusText("Network UMP : inviting remote device...");
                    if (SessionHasClosed)
                    {
                        this->StatusBar->SetStatusText("Network UMP : remote device closed connection or refuse it...");
                    }
                    break;
                case 3 :
                    this->StatusBar->SetStatusText("Network UMP : connected");
                    break;
                default : this->StatusBar->SetStatusText("Network UMP has an unknown status !");
            }
        }
    }
}  // wxFluidMainFrame::OnRefreshTimer
// -----------------------------------------------------

void wxFluidMainFrame::OnMenuClearPresets(wxCommandEvent& WXUNUSED(event))
{
    if (ConfigChanged)
    {
        if (wxMessageBox("Presets configuration has not been saved!\nAre you sure you want to clear current presets ?", "Clear presets", wxYES_NO+wxNO_DEFAULT+wxICON_WARNING)!=wxYES)
            return;
    }

    ConfigChanged = false;
	GSynth->ClearPresets();
	this->SetTitle("wxFluid");
	this->UpdateFrontPanel();
}  // wxFluidMainFrame::OnMenuClearPresets
// -----------------------------------------------------

void wxFluidMainFrame::OnMenuLoadPresets(wxCommandEvent& WXUNUSED(event))
{
	wxFileName FileName;
	FILE* LastPresetFile;

	if (ConfigChanged)
    {
        if (wxMessageBox("Presets configuration has not been saved!\nAre you sure you want to load new presets ?", "Load presets", wxYES_NO+wxNO_DEFAULT+wxICON_WARNING)!=wxYES)
            return;
    }

	wxFileDialog dialog(this, wxT("Load wxFluid presets"), wxEmptyString, wxEmptyString, wxT("wxFluid presets (*.wxf)|*.wxf;*.WXF"), wxFD_OPEN + wxFD_FILE_MUST_EXIST);
	if (dialog.ShowModal() == wxID_OK)
	{
		this->PresetFileName = dialog.GetPath();
		GSynth->LoadPresetFile(this->PresetFileName);

		ConfigChanged = false;
		FileName = this->PresetFileName;
		this->SetTitle("wxFluid - " + FileName.GetName());
		this->UpdateFrontPanel();

		// Save the current preset filename so it will be loaded automatically next time wxFluid starts
		LastPresetFile = fopen("lastpreset.dat", "wt");
		if (LastPresetFile != 0)
		{  // The file contains the full path to the wxFluid preset file
			fprintf(LastPresetFile, "%s", this->PresetFileName.ToUTF8().data());
			fclose(LastPresetFile);
		}
	}
}  // wxFluidMainFrame::OnMenuLoadPresets
// -----------------------------------------------------

void wxFluidMainFrame::OnMenuSavePresets(wxCommandEvent& event)
{
	if (this->PresetFileName != "")
	{  // Preset file name is known
        ConfigChanged = false;
		GSynth->SavePresetConfiguration(this->PresetFileName);
	}
	else
	{  // Filename is not defined : force the dialog "Save as" to open
		this->OnMenuSavePresetsAs(event);
	}
}  // wxFluidMainFrame::OnMenuSaveSound
// -----------------------------------------------------

void wxFluidMainFrame::OnMenuSavePresetsAs(wxCommandEvent& WXUNUSED(event))
{
	wxFileName FileName;
	FILE* LastPresetFile;

	wxFileDialog dialog(this, wxT("Save wxFluid presets"), wxEmptyString, wxEmptyString, wxT("wxFluid presets (*.wxf)|*.wxf;*.WXF"), wxFD_SAVE);
	if (dialog.ShowModal() == wxID_OK)
	{
		this->PresetFileName = dialog.GetPath();
		GSynth->SavePresetConfiguration(this->PresetFileName);

        ConfigChanged = false;
		FileName = this->PresetFileName;
		this->SetTitle("wxFluid - " + FileName.GetName());

		// Save the current preset filename so it will be loaded automatically next time wxFluid starts
		LastPresetFile = fopen("lastpreset.dat", "wt");
		if (LastPresetFile != 0)
		{  // The file contains the full path to the wxFluid preset file
			fprintf(LastPresetFile, "%s", this->PresetFileName.ToUTF8().data());
			fclose(LastPresetFile);
		}
	}
}  // wxFluidMainFrame::OnMenuSavePresetsAs
// -----------------------------------------------------

void wxFluidMainFrame::OnMenuConfigureSynth(wxCommandEvent& WXUNUSED(event))
{
	CFluidConfigDialog* Dialog;

	Dialog = new CFluidConfigDialog(this, &GSynth->Config);
	if (Dialog)
	{
		if (Dialog->ShowModal() == wxID_OK)
		{
			// Configuration has been changed : FluidSynth instance must be restarted
			wxMessageBox("wxFluid configuration has been changed.\nThe synthesizer will now restart", "Configuration changed", wxOK + wxICON_INFORMATION);
			GSynth->SaveEngineConfiguration();
			GSynth->StopSynth();
			GSynth->CreateFluidEngine();
		}
		delete Dialog;
	}
}  // wxFluidMainFrame::OnMenuConfigureSynth
// -----------------------------------------------------

void wxFluidMainFrame::OnMenuConfigureMIDI(wxCommandEvent& WXUNUSED(event))
{
	CFluidMIDIConfigDialog* Dialog;

	Dialog = new CFluidMIDIConfigDialog(this, &GSynth->Config);
	if (Dialog)
	{
		if (Dialog->ShowModal() == wxID_OK)
		{  // Configuration has changed : we need only to restart the MIDI interface
			GSynth->StopMIDI();
			GSynth->SaveEngineConfiguration();
			wxMessageBox("wxFluid MIDI configuration has been changed.\nRestarting MIDI interface...", "Configuration changed", wxOK + wxICON_INFORMATION);
			GSynth->StartMIDI();
		}
		delete Dialog;
	}
}  // wxFluidMainFrame::OnMenuConfigureMIDI
// -----------------------------------------------------

void wxFluidMainFrame::OnViewVirtualKeyboard(wxCommandEvent& WXUNUSED(event))
{
	if (VirtualKeyboard)
	{
		// The Hide/Show trick allows to put back the frame in front
		VirtualKeyboard->Hide();
		VirtualKeyboard->Show();
	}
}  // wxFluidMainFrame::OnViewVirtualKeyboard
// -----------------------------------------------------

void wxFluidMainFrame::OnViewMIDIControls(wxCommandEvent& WXUNUSED(event))
{
	/*
	if (this->MIDIControllers)
	{
		this->MIDIControllers->Hide();
		this->MIDIControllers->Show();
	}
	*/
}  // wxFluidMainFrame::OnViewMIDIControls
// -----------------------------------------------------

void wxFluidMainFrame::OnConfigureSoundFontsPresets(wxCommandEvent& WXUNUSED(event))
{
	CSoundFontPresetManager* Dialog;

	Dialog = new CSoundFontPresetManager(this, GSynth);
	if (Dialog)
	{
		Dialog->ShowModal();
		delete Dialog;
	}
}  // wxFluidMainFrame::OnConfigureSoundFontsPresets
// -----------------------------------------------------

void wxFluidMainFrame::OnVolumeKnob(wxCommandEvent& WXUNUSED(event))
{
    ConfigChanged = true;
    GSynth->SetVolume (this->VolumeKnob->GetValue());
}  // wxFluidMainFrame::OnVolumeKnob
// -----------------------------------------------------

void wxFluidMainFrame::OnReverbRoomSizeKnob(wxCommandEvent& WXUNUSED(event))
{
    ConfigChanged = true;
	GSynth->SetReverbRoomSize(this->ReverbRoomSizeKnob->GetValue());
}  // wxFluidMainFrame::OnReverbRoomSizeKnob
// -----------------------------------------------------

void wxFluidMainFrame::OnReverbDampingKnob(wxCommandEvent& WXUNUSED(event))
{
    ConfigChanged = true;
	GSynth->SetReverbDamping(this->ReverbDampingKnob->GetValue());
}  // wxFluidMainFrame::OnReverbDampingKnob
// -----------------------------------------------------

void wxFluidMainFrame::OnReverbWidthKnob(wxCommandEvent& WXUNUSED(event))
{
    ConfigChanged = true;
	GSynth->SetReverbWidth(this->ReverbWidthKnob->GetValue());
}  // wxFluidMainFrame::OnReverbWidthKnob
// -----------------------------------------------------

void wxFluidMainFrame::OnReverbLevelKnob(wxCommandEvent& WXUNUSED(event))
{
    ConfigChanged = true;
	GSynth->SetReverbLevel(this->ReverbLevelKnob->GetValue());
}  // wxFluidMainFrame::OnReverbLevelKnob
// -----------------------------------------------------

void wxFluidMainFrame::OnChorusNKnob(wxCommandEvent& WXUNUSED(event))
{
    ConfigChanged = true;
	GSynth->SetChorusNumber(this->ChorusNKnob->GetValue()*99.0);
}  // wxFluidMainFrame::OnChorusNKnob
// -----------------------------------------------------

void wxFluidMainFrame::OnChorusLevelKnob(wxCommandEvent& WXUNUSED(event))
{
    ConfigChanged = true;
	GSynth->SetChorusLevel(this->ChorusLevelKnob->GetValue());
}  // wxFluidMainFrame::OnChorusLevelKnob
// -----------------------------------------------------

void wxFluidMainFrame::OnChorusSpeedKnob(wxCommandEvent& WXUNUSED(event))
{
    ConfigChanged = true;
	GSynth->SetChorusSpeed(this->ChorusSpeedKnob->GetValue());
}  // wxFluidMainFrame::OnChorusSpeedKnob
// -----------------------------------------------------

void wxFluidMainFrame::OnChorusDepthKnob(wxCommandEvent& WXUNUSED(event))
{
    ConfigChanged = true;
	GSynth->SetChorusDepth(ChorusDepthKnob->GetValue());
}  // wxFluidMainFrame::OnChorusDepthKnob
// -----------------------------------------------------

void wxFluidMainFrame::OnReverbOnOff(wxCommandEvent& WXUNUSED(event))
{
    ConfigChanged = true;
	GSynth->SetReverbState(this->ReverbOnToggle->GetValue());
}  // wxFluidMainFrame::OnReverbOnOff
// -----------------------------------------------------

void wxFluidMainFrame::OnChorusOnOff(wxCommandEvent& WXUNUSED(event))
{
    ConfigChanged = true;
	GSynth->SetChorusState(this->ChorusOnToggle->GetValue());
}  // wxFluidMainFrame::OnChorusOnOff
// -----------------------------------------------------

void wxFluidMainFrame::OnChorusWave(wxCommandEvent& WXUNUSED(event))
{
    ConfigChanged = true;
	GSynth->SetChorusWave(this->ChorusWaveToggle->GetValue());
}  // wxFluidMainFrame::OnChorusWave
// -----------------------------------------------------

void wxFluidMainFrame::UpdateFrontPanel(void)
{
	this->VolumeKnob->SetValue(GSynth->VolumeControl);

	this->ReverbOnToggle->SetValue(GSynth->ReverbControlOn);
	this->ReverbRoomSizeKnob->SetValue(GSynth->ReverbRoomSizeControl);
	this->ReverbDampingKnob->SetValue(GSynth->ReverbDampingControl);
	this->ReverbWidthKnob->SetValue(GSynth->ReverbWidthControl);
	this->ReverbLevelKnob->SetValue(GSynth->ReverbLevelControl);

	this->ChorusOnToggle->SetValue(GSynth->ChorusControlOn);
	this->ChorusNKnob->SetValue((float)GSynth->ChorusNumberControl*0.01f);
	this->ChorusSpeedKnob->SetValue(GSynth->ChorusSpeedControl);
	this->ChorusDepthKnob->SetValue(GSynth->ChorusDepthControl);
	this->ChorusLevelKnob->SetValue(GSynth->ChorusLevelControl);
	this->ChorusWaveToggle->SetValue(GSynth->ChorusWaveTriangle);
}  // wxFluidMainFrame::UpdateFrontPanel
// -----------------------------------------------------

