/////////////////////////////////////////////////////////////////////////////
// Name:        ConfigurationDialog.h
// Purpose:     wxFluid configuration dialog
// Author:      Benoit BOUCHEZ
// Created:
// Copyright:   (c) Benoit BOUCHEZ
/////////////////////////////////////////////////////////////////////////////

#include "SynthConfigurationDialog.h"
#ifdef __TARGET_WIN__
#include <mmsystem.h>
#include "RtAudio.h"
#endif
#ifdef __TARGET_LINUX__
#include <alsa/asoundlib.h>
#endif // __TARGET_LINUX__

wxString EmptyName = "";

CFluidConfigDialog::CFluidConfigDialog(wxWindow* Parent, TWXFLUIDCONFIG* Config) : CSynthConfigDialog(Parent)
{
	// Set all widgets to values from configuration data
	this->PConfig = Config;

#ifdef __TARGET_WIN__
    AudioDriverChoice->Clear();
	AudioDriverChoice->Append("MME");
	AudioDriverChoice->Append("DirectSound");
	AudioDriverChoice->Append("WASAPI");
    AudioDriverChoice->SetSelection(Config->AudioDriverType);
#endif

#ifdef __TARGET_LINUX__
    AudioDriverChoice->Clear();
	AudioDriverChoice->Append("JACK");
	AudioDriverChoice->Append("ALSA");
	TxtWASAPI->Hide();
	ChkAudioWasapiExclusive->Hide();
    AudioDriverChoice->SetSelection(Config->AudioDriverType);
#endif
    
#ifdef __TARGET_MAC__
    AudioDriverChoice->Clear();
    AudioDriverChoice->Append("CoreAudio");     // Only CoreAudio is available on Mac
    AudioDriverChoice->Enable(false);
    AudioDriverChoice->SetSelection(0);
    TxtWASAPI->Hide();
    ChkAudioWasapiExclusive->Hide();
#endif


	// Fill the port list depending on selected driver
#ifdef __TARGET_WIN__
	switch (Config->AudioDriverType)
	{
	case WINDOWS_MME_DRIVER: this->FillMMEDeviceList(Config->AudioDeviceName); break;
	case WINDOWS_DIRECTSOUND_DRIVER: this->FillDirectSoundDeviceList(Config->AudioDeviceName); break;
	case WINDOWS_WASAPI_DRIVER: this->FillWASAPIDeviceList(Config->AudioDeviceName); break;
	}
#endif

#ifdef __TARGET_LINUX__
    switch (Config->AudioDriverType)
    {
    case LINUX_JACK_DRIVER : this->SelectJACK(); break;
    case LINUX_ALSA_DRIVER : this->ListALSADevices(Config->AudioDeviceName); break;
    }
#endif // __TARGET_LINUX__
    
#ifdef __TARGET_MAC__
    this->FillCoreAudioDeviceList(Config->AudioDeviceName);
#endif

	switch (Config->AudioBufferSize)
	{
	case 64: AudioBufferSizeChoice->SetSelection(0); break;
	case 128: AudioBufferSizeChoice->SetSelection(1); break;
	case 256: AudioBufferSizeChoice->SetSelection(2); break;
	case 1024: AudioBufferSizeChoice->SetSelection(4); break;
	case 2048: AudioBufferSizeChoice->SetSelection(5); break;
	case 4096: AudioBufferSizeChoice->SetSelection(6); break;
	case 8192: AudioBufferSizeChoice->SetSelection(7); break;
	default: AudioBufferSizeChoice->SetSelection(3);		// 512
	}
	this->AudioPeriods->SetValue(Config->AudioPeriods);
	ChkAudioWasapiExclusive->SetValue(Config->ExclusiveWasapi);

	switch (Config->SampleRate)
	{
	case 48000: SampleRateChoice->SetSelection(1); break;
	case 88200: SampleRateChoice->SetSelection(2); break;
	case 96000: SampleRateChoice->SetSelection(3); break;
	case 192000: SampleRateChoice->SetSelection(4); break;
	default: SampleRateChoice->SetSelection(0);		// 44.1kHz
	}
}  // CFluidConfigDialog::CFluidConfigDialog
// -----------------------------------------------------

void CFluidConfigDialog::OnOKBtn(wxCommandEvent& WXUNUSED(event))
{
	// Save parameters from dialog to config storage
	PConfig->AudioDriverType = AudioDriverChoice->GetSelection();
	PConfig->AudioDeviceName = AudioDeviceChoice->GetStringSelection();
	switch (AudioBufferSizeChoice->GetSelection())
	{
	case 0: PConfig->AudioBufferSize = 64; break;
	case 1: PConfig->AudioBufferSize = 128; break;
	case 2: PConfig->AudioBufferSize = 256; break;
	case 3: PConfig->AudioBufferSize = 512; break;
	case 4: PConfig->AudioBufferSize = 1024; break;
	case 5: PConfig->AudioBufferSize = 2048; break;
	case 6: PConfig->AudioBufferSize = 4096; break;
	case 7: PConfig->AudioBufferSize = 8192; break;
	}
	PConfig->AudioPeriods = this->AudioPeriods->GetValue();

	switch (SampleRateChoice->GetSelection())
	{
	case 0: PConfig->SampleRate = 44100; break;
	case 1: PConfig->SampleRate = 48000; break;
	case 2: PConfig->SampleRate = 88200; break;
	case 3: PConfig->SampleRate = 96000; break;
	case 4: PConfig->SampleRate = 192000; break;
	}

#ifdef __TARGET_WIN__
	PConfig->ExclusiveWasapi = ChkAudioWasapiExclusive->GetValue();
#endif

	EndModal(wxID_OK);
}  // CFluidConfigDialog::OnOKBtn
// -----------------------------------------------------

void CFluidConfigDialog::OnCancelBtn(wxCommandEvent& WXUNUSED(event))
{
	EndModal(wxID_CANCEL);
}  // CFluidConfigDialog::OnCancelBtn
// -----------------------------------------------------

#ifdef __TARGET_WIN__
void CFluidConfigDialog::FillMMEDeviceList(wxString& SelectedDeviceName)
{
	WAVEOUTCAPS WaveOutCaps;
	wxString wxDeviceName;
	int SelectionIndex = -1;
	int OutputDeviceCount = 0;

	this->AudioDeviceChoice->Clear();

	if (waveOutGetNumDevs() == 0) return;

	for (unsigned int i = 0; i < waveOutGetNumDevs(); i++)
	{
		waveOutGetDevCaps(i, &WaveOutCaps, sizeof(WAVEOUTCAPS));
		wxDeviceName = WaveOutCaps.szPname;
		AudioDeviceChoice->Append(wxDeviceName);
		if (wxDeviceName == SelectedDeviceName)
			SelectionIndex = OutputDeviceCount;

		OutputDeviceCount += 1;
	}

	if (SelectionIndex != -1)
		AudioDeviceChoice->SetSelection(SelectionIndex);
}  // CFluidConfigDialog::FillMMEDeviceList
// -----------------------------------------------------

int CFluidConfigDialog::FillDirectSoundDeviceList(wxString& SelectedDeviceName)
{
	RtAudio* AudioDeviceEnumerator;
	int DeviceCount;
	std::vector< unsigned int > IDList;
	RtAudio::DeviceInfo DeviceInfo;
	int OutputDeviceCount = 0;
	wxString wxDeviceName;
	int SelectionIndex = -1;

	this->AudioDeviceChoice->Clear();

	AudioDeviceEnumerator = new RtAudio(RtAudio::WINDOWS_DS, 0);
	if (AudioDeviceEnumerator)
	{
		DeviceCount = AudioDeviceEnumerator->getDeviceCount();
		if (DeviceCount > 0)
		{
			IDList = AudioDeviceEnumerator->getDeviceIds();
			for (int i = 0; i < DeviceCount; i++)
			{
				DeviceInfo = AudioDeviceEnumerator->getDeviceInfo(IDList.at(i));

				// Count the device only if it has outputs
				if ((DeviceInfo.outputChannels > 0) || (DeviceInfo.duplexChannels > 0))
				{
					wxDeviceName = DeviceInfo.name;
					AudioDeviceChoice->Append(wxDeviceName);
					if (wxDeviceName == SelectedDeviceName)
						SelectionIndex = OutputDeviceCount;

					OutputDeviceCount += 1;
				}
			}
		}

		delete AudioDeviceEnumerator;
	}

	if (SelectionIndex != -1)
		AudioDeviceChoice->SetSelection(SelectionIndex);

	return OutputDeviceCount;
}  // CFluidConfigDialog::FillDirectSoundDeviceList
// -----------------------------------------------------

int CFluidConfigDialog::FillWASAPIDeviceList(wxString& SelectedDeviceName)
{
	RtAudio* AudioDeviceEnumerator;
	int DeviceCount;
	RtAudio::DeviceInfo DeviceInfo;
	std::vector< unsigned int > IDList;
	int OutputDeviceCount = 0;
	int SelectionIndex = -1;
	wxString wxDeviceName;

	this->AudioDeviceChoice->Clear();

	AudioDeviceEnumerator = new RtAudio(RtAudio::WINDOWS_WASAPI, 0);
	if (AudioDeviceEnumerator)
	{
		DeviceCount = AudioDeviceEnumerator->getDeviceCount();
		if (DeviceCount > 0)
		{
			IDList = AudioDeviceEnumerator->getDeviceIds();
			for (int i = 0; i < DeviceCount; i++)
			{
				DeviceInfo = AudioDeviceEnumerator->getDeviceInfo(IDList.at(i));

				// Count the device only if it has outputs
				if ((DeviceInfo.outputChannels > 0) || (DeviceInfo.duplexChannels > 0))
				{
					wxDeviceName = DeviceInfo.name;		// Cast into wxString
					if (wxDeviceName == SelectedDeviceName)
						SelectionIndex = OutputDeviceCount;
					AudioDeviceChoice->Append(DeviceInfo.name);
					OutputDeviceCount += 1;
				}
			}
		}

		delete AudioDeviceEnumerator;
	}

	if (SelectionIndex != -1)
		AudioDeviceChoice->SetSelection(SelectionIndex);

	return OutputDeviceCount;
}  // CFluidConfigDialog::FillWASAPIDeviceList
// -----------------------------------------------------
#endif

#ifdef __TARGET_LINUX__
void CFluidConfigDialog::SelectJACK(void)
{  // Disable device list (there is no possible selection for JACK)
	this->AudioDeviceChoice->Clear();
	this->AudioDeviceChoice->Append("");
	this->AudioDeviceChoice->Enable(false);
}  // CFluidConfigDialog::SelectJACK
// -----------------------------------------------------

int CFluidConfigDialog::ListALSADevices(wxString& SelectedDeviceName)
{
/*
	void** hints;
	void** n;
	char* io;
	char* name;
*/

	int card;
	char name[32];
	snd_ctl_card_info_t* info;
	const char* pname;
	int err;
	snd_ctl_t* handle;
	wxString wxDeviceName;

	int OutputDeviceCount = 0;

	this->AudioDeviceChoice->Clear();
	this->AudioDeviceChoice->Enable(true);

	// Following code lists PCM devices
	/*
	if (snd_device_name_hint(-1, "pcm", &hints) < 0)
		return OutputDeviceCount;

	n = hints;

	while (*n != NULL)
	{
		name = snd_device_name_get_hint(*n, "NAME");
		io = snd_device_name_get_hint(*n, "IOID");

		if ((io != NULL) && (name != NULL))
		{
			if (strcmp(io, "Output") == 0)
			{
				this->AudioDeviceChoice->Append(name);
				OutputDeviceCount += 1;
			}
		}

		if (name != NULL)
			free(name);
		if (io != NULL)
			free(io);

		n++;
	}

	snd_device_name_free_hint(hints);
	*/

	// Following code list hardware devices
	snd_ctl_card_info_alloca(&info);

	card = -1;
	if (snd_card_next(&card)<0) return 0;  // No soundcard found
	if (card < 0) return 0;  // No soundcard found

	while (card >= 0)
	{
        sprintf (name, "hw:%d", card);

        err = snd_ctl_open(&handle, name, 0);
        if (err < 0) goto next_card;

        err = snd_ctl_card_info(handle, info);
        if (err < 0)
        {
            snd_ctl_close(handle);
            goto next_card;
        }

        pname = snd_ctl_card_info_get_name(info);
        // Name to be passed to fluidsynth is "hw:0", "hw:1", etc...
        // We display here the name for each possible index
        wxDeviceName = "hw:" + wxString::Format("%d", card) + " - " + wxString::Format ("%s", pname);
        this->AudioDeviceChoice->Append (wxDeviceName);

        OutputDeviceCount+=1;

        // Go to next card in list
        next_card:
        if (snd_card_next(&card) < 0)
        {
            break;
        }
	}

	return OutputDeviceCount;
}  // CFluidConfigDialog::ListALSADevices
// -----------------------------------------------------
#endif

#ifdef __TARGET_MAC__
int CFluidConfigDialog::FillCoreAudioDeviceList(wxString& SelectedDeviceName)
{
    int OutputDeviceCount = 1;      // TODO : set to 0 when RtAudio will be used
    
    // For now, we only set "Default" device
    // TODO : using RtAudio, list all CoreAudio devices
    this->AudioDeviceChoice->Clear();
    this->AudioDeviceChoice->Append ("default");
    this->AudioDeviceChoice->SetSelection(0);
    
    return OutputDeviceCount;
}  // CFluidConfigDialog::FillCoreAudioDeviceList
// -----------------------------------------------------
#endif

void CFluidConfigDialog::OnAudioDriverChoice(wxCommandEvent& WXUNUSED(event))
{
	// If a new driver is selected, update the list of ports for the selected driver
	AudioDeviceChoice->Clear();

#ifdef __TARGET_WIN__
	// As MME is not supported by RtAudio, we use the MME API to list all ports
	if (AudioDriverChoice->GetSelection() == WINDOWS_MME_DRIVER)
	{  // MME WaveOut
		if (waveOutGetNumDevs() == 0)
		{
			wxMessageBox("No MME WaveOut device available on this computer", "MME Error", wxOK + wxICON_WARNING);
		}
		else
		{
			this->FillMMEDeviceList(EmptyName);
		}
	}
	else if (AudioDriverChoice->GetSelection() == WINDOWS_DIRECTSOUND_DRIVER)
	{  // DirectSound
		if (this->FillDirectSoundDeviceList(EmptyName) == 0)
		{
			wxMessageBox("No DirectSound output device available on this computer", "DirectSound Error", wxOK + wxICON_WARNING);
		}
	}
	else
	{  // WASAPI
		if (this->FillWASAPIDeviceList(EmptyName) == WINDOWS_WASAPI_DRIVER)
		{
			wxMessageBox("No WASAPI output device available on this computer", "WASAPI Error", wxOK + wxICON_WARNING);
		}
	}

	AudioDeviceChoice->SetSelection(0);
#endif

#ifdef __TARGET_LINUX__
	if (AudioDriverChoice->GetSelection() == LINUX_JACK_DRIVER)
	{  // JACK
        this->SelectJACK();
    }
    else
    {  // ALSA
        if (ListALSADevices(EmptyName) == 0)
        {
            wxMessageBox ("No ALSA PCM output device available on this computer", "ALSA Error", wxOK + wxICON_WARNING);
        }
    }
#endif
}  // CFluidConfigDialog::OnAudioDriverChoice
// -----------------------------------------------------
