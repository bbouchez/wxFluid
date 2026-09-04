/*
 *  AudioConfigurationDialog.cpp
 *  wxFluid Synthesizer
 *  Audio engine configuration dialog
 *
 * Copyright (c) 2026 Benoit BOUCHEZ
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

#include "AudioConfigurationDialog.h"
#ifdef __TARGET_WIN__
//#include <mmsystem.h>
#include "RtAudio.h"
#endif
#ifdef __TARGET_LINUX__
#include <alsa/asoundlib.h>
#endif // __TARGET_LINUX__

wxString EmptyName = "";

CAudioEngineConfigDialog::CAudioEngineConfigDialog(wxWindow* Parent, TWXFLUIDCONFIG* Config) : CAudioConfigDialog(Parent)
{
	// Set all widgets to values from configuration data
	this->PConfig = Config;

#ifdef __TARGET_WIN__
    this->AudioDriverChoice->Clear();
	this->AudioDriverChoice->Append("WASAPI");
	this->AudioDriverChoice->Append("ASIO");
    this->AudioDriverChoice->SetSelection(Config->AudioDriverType);
#endif

#ifdef __TARGET_LINUX__
    this->AudioDriverChoice->Clear();
	this->AudioDriverChoice->Append("JACK");
	this->AudioDriverChoice->Append("ALSA");
	this->TxtWASAPI->Hide();
	this->ChkAudioWasapiExclusive->Hide();
    this->AudioDriverChoice->SetSelection(Config->AudioDriverType);
#endif
    
#ifdef __TARGET_MAC__
    this->AudioDriverChoice->Clear();
    this->AudioDriverChoice->Append("CoreAudio");     // Only CoreAudio is available on Mac
    this->AudioDriverChoice->Enable(false);
    this->AudioDriverChoice->SetSelection(0);
    this->TxtWASAPI->Hide();
    this->ChkAudioWasapiExclusive->Hide();
#endif


	// Fill the port list depending on selected driver
#ifdef __TARGET_WIN__
	switch (Config->AudioDriverType)
	{
	case WINDOWS_WASAPI_DRIVER: this->FillWASAPIDeviceList(Config->AudioDeviceName); break;
	case WINDOWS_ASIO_DRIVER: this->FillASIODeviceList(Config->AudioDeviceName); break;
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

	this->EditBufferSize->SetValue(wxString::Format("%d", Config->AudioBufferSize));
	//this->AudioPeriods->SetValue(Config->AudioPeriods);
	ChkAudioWasapiExclusive->SetValue(Config->ExclusiveWasapi);

	switch (Config->SampleRate)
	{
	case 48000: SampleRateChoice->SetSelection(1); break;
	case 88200: SampleRateChoice->SetSelection(2); break;
	case 96000: SampleRateChoice->SetSelection(3); break;
	case 192000: SampleRateChoice->SetSelection(4); break;
	default: SampleRateChoice->SetSelection(0);		// 44.1kHz
	}
}  // CAudioEngineConfigDialog::CFluidConfigDialog
// -----------------------------------------------------

void CAudioEngineConfigDialog::OnOKBtn(wxCommandEvent& WXUNUSED(event))
{
	// Save parameters from dialog to config storage
	PConfig->AudioDriverType = AudioDriverChoice->GetSelection();
	PConfig->AudioDeviceName = AudioDeviceChoice->GetStringSelection();

	PConfig->AudioBufferSize = wxAtoi(this->EditBufferSize->GetValue());
	//PConfig->AudioPeriods = this->AudioPeriods->GetValue();

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
}  // CAudioEngineConfigDialog::OnOKBtn
// -----------------------------------------------------

void CAudioEngineConfigDialog::OnCancelBtn(wxCommandEvent& WXUNUSED(event))
{
	EndModal(wxID_CANCEL);
}  // CAudioEngineConfigDialog::OnCancelBtn
// -----------------------------------------------------

#ifdef __TARGET_WIN__
int CAudioEngineConfigDialog::FillWASAPIDeviceList(wxString& SelectedDeviceName)
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
}  // CAudioEngineConfigDialog::FillWASAPIDeviceList
// -----------------------------------------------------

int CAudioEngineConfigDialog::FillASIODeviceList(wxString& SelectedDeviceName)
{
	RtAudio* AudioDeviceEnumerator;
	int DeviceCount;
	RtAudio::DeviceInfo DeviceInfo;
	std::vector< unsigned int > IDList;
	int OutputDeviceCount = 0;
	wxString wxDeviceName;
	int SelectionIndex = -1;

	this->AudioDeviceChoice->Clear();

	AudioDeviceEnumerator = new RtAudio(RtAudio::WINDOWS_ASIO, 0);
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
}  // CAudioEngineConfigDialog::FillASIODeviceList
// -----------------------------------------------------
#endif

#ifdef __TARGET_LINUX__
void CAudioEngineConfigDialog::SelectJACK(void)
{  // Disable device list (there is no possible selection for JACK)
	this->AudioDeviceChoice->Clear();
	this->AudioDeviceChoice->Append("");
	this->AudioDeviceChoice->Enable(false);
}  // CAudioEngineConfigDialog::SelectJACK
// -----------------------------------------------------

int CAudioEngineConfigDialog::ListALSADevices(wxString& SelectedDeviceName)
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
}  // CAudioEngineConfigDialog::ListALSADevices
// -----------------------------------------------------
#endif

#ifdef __TARGET_MAC__
int CAudioEngineConfigDialog::FillCoreAudioDeviceList(wxString& SelectedDeviceName)
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

void CAudioEngineConfigDialog::OnAudioDriverChoice(wxCommandEvent& WXUNUSED(event))
{
	// If a new driver is selected, update the list of ports for the selected driver
	AudioDeviceChoice->Clear();

#ifdef __TARGET_WIN__
	if (this->AudioDriverChoice->GetSelection()==0)
	{  // WASAPI
		if (this->FillWASAPIDeviceList(EmptyName) == 0)		// No WASAPI device found
		{
			wxMessageBox("No WASAPI output device available on this computer", "WASAPI Error", wxOK + wxICON_WARNING);
		}
	}
	else
	{
		if (this->FillASIODeviceList(EmptyName) == 0)		// No ASIO device found
		{
			wxMessageBox("No ASIO output device available on this computer", "ASIO Error", wxOK + wxICON_WARNING);
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
}  // CAudioEngineConfigDialog::OnAudioDriverChoice
// -----------------------------------------------------
