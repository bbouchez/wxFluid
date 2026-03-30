/*
 *  MIDIConfigurationDialog.cpp
 *  wxFluid Synthesizer
 *  wxFluid MIDI / RTP-MIDI / Network MIDI 2.0 configuration dialog
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

#include "MIDIConfigurationDialog.h"
#include "RtMidi.h"
#ifdef __TARGET_MAC__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#ifdef __TARGET_LINUX__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

CFluidMIDIConfigDialog::CFluidMIDIConfigDialog(wxWindow* Parent, TWXFLUIDCONFIG* Config) : CMIDIConfigDialog (Parent)
{
	RtMidiIn* MIDIIn = 0;
    int SelectionIndex = -1;
	struct in_addr ip_addr;

	// Set all widgets to values from configuration data
	this->PConfig = Config;

	// Fill the MIDI list with devices names
	// TODO : if device name matches configuration, set wxChoice to correct position
	MIDIIn = new RtMidiIn;
	if (MIDIIn->getPortCount() > 0)
	{
		for (unsigned int i = 0; i < MIDIIn->getPortCount(); i++)
		{
            if (MIDIIn->getPortName(i)==Config->MIDIInputName)
                SelectionIndex = i;
			MIDIDeviceChoice->Append(MIDIIn->getPortName(i));
		}
        if (SelectionIndex<0)
        {  // Device name from config file not found : use first one by default
            MIDIDeviceChoice->SetSelection(0);
        }
        else
        {
            MIDIDeviceChoice->SetSelection(SelectionIndex);
        }
	}

	this->ChkUseMIDI->SetValue(Config->UseMIDIInput);

	this->ChkSessionInitiator->SetValue(Config->IsSessionInitiator);
	this->RadioNetworkMIDIMode->SetSelection(Config->NetworkMIDIMode);
	this->EditLocalUDPPort->SetValue(wxString::Format("%d", Config->LocalUDPPort));
	this->EditDeviceUDPPort->SetValue(wxString::Format("%d", Config->RemoteUDPPort));

	ip_addr.s_addr = htonl(Config->RemoteDeviceIP);
	this->EditDeviceIP->SetValue(inet_ntoa(ip_addr));

#ifdef __TARGET_LINUX__
    this->SetClientSize(700, 460);
    this->EditDeviceIP->SetMinSize(wxSize(160, -1));
#endif // __TARGET_LINUX__

	delete MIDIIn;
}  // CFluidMIDIConfigDialog::CFluidMIDIConfigDialog
// -----------------------------------------------------

void CFluidMIDIConfigDialog::OnOKBtn(wxCommandEvent& WXUNUSED(event))
{
	int Test;
	unsigned long IPV4;

	PConfig->UseMIDIInput = this->ChkUseMIDI->GetValue();
	PConfig->MIDIInputName = MIDIDeviceChoice->GetStringSelection();
	PConfig->NetworkMIDIMode = this->RadioNetworkMIDIMode->GetSelection();
	PConfig->IsSessionInitiator = this->ChkSessionInitiator->GetValue();

	Test = wxAtoi(this->EditLocalUDPPort->GetValue());
	if ((Test < 0) || (Test > 65535))
	{
		wxMessageBox("Invalid local UDP port entered.\nDefault value has been restored.", "Error", wxOK + wxICON_EXCLAMATION);
		Test = 6004;		// Restore the default value
	}
	PConfig->LocalUDPPort = Test;

	Test = wxAtoi(this->EditDeviceUDPPort->GetValue());
	if ((Test < 0) || (Test > 65535))
	{
		wxMessageBox("Invalid device UDP port entered.\nDefault value has been restored.", "Error", wxOK + wxICON_EXCLAMATION);
		Test = 5004;		// Restore the default value
	}
	PConfig->RemoteUDPPort = Test;

	IPV4 = inet_addr(EditDeviceIP->GetValue().mb_str());
	if (IPV4 == INADDR_NONE)
	{
		wxMessageBox("Invalid IP address", "Remote peer IP address", wxOK + wxICON_ERROR);
		PConfig->RemoteDeviceIP = 0x7F000001;
	}
	else PConfig->RemoteDeviceIP = htonl(IPV4);

	EndModal(wxID_OK);
}  // CFluidMIDIConfigDialog::OnOKBtn
// -----------------------------------------------------

void CFluidMIDIConfigDialog::OnCancelBtn(wxCommandEvent& WXUNUSED(event))
{
	EndModal(wxID_CANCEL);
}  // CFluidMIDIConfigDialog::OnCancelBtn
// -----------------------------------------------------
