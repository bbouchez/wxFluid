/////////////////////////////////////////////////////////////////////////////
// EngineConfigFile.cpp
// Methods to save and load wxFluid engine configuration
// (c) Benoit BOUCHEZ
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <wx/xml/xml.h>
#include "SynthClass.h"
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

static void LoadSynthesizer(wxXmlNode* ParentNode, CwxFluidSynth* Synth)
{
	wxXmlNode* SynthNode;
	wxString AttrStr;
	wxString RemoteIP;

	SynthNode = ParentNode->GetChildren();
	if (SynthNode != 0)
	{
		do
		{
			if (SynthNode->GetName() == "audio")
			{
				SynthNode->GetAttribute("driver", &AttrStr);
				if (AttrStr == "1") Synth->Config.AudioDriverType = 1;
				else if (AttrStr == "2") Synth->Config.AudioDriverType = 2;
				else Synth->Config.AudioDriverType = 0;

				SynthNode->GetAttribute("device", &Synth->Config.AudioDeviceName);

				SynthNode->GetAttribute("buffers", &AttrStr);
				Synth->Config.AudioPeriods = wxAtoi(AttrStr);

				SynthNode->GetAttribute("buffersize", &AttrStr);
				Synth->Config.AudioBufferSize = wxAtoi(AttrStr);

#ifdef __TARGET_WIN__
				SynthNode->GetAttribute("exclusivewasapi", &AttrStr);
				if (AttrStr == "1") Synth->Config.ExclusiveWasapi = true;
				else Synth->Config.ExclusiveWasapi = false;
#endif

				SynthNode->GetAttribute("samplerate", &AttrStr);
				Synth->Config.SampleRate = wxAtoi(AttrStr);
			}
			else if (SynthNode->GetName() == "midi")
			{
				SynthNode->GetAttribute("active", &AttrStr);
				if (AttrStr == "1") Synth->Config.UseMIDIInput = true;
				else Synth->Config.UseMIDIInput = false;

				SynthNode->GetAttribute("device", &Synth->Config.MIDIInputName);

				SynthNode->GetAttribute("channels", &AttrStr);
				Synth->Config.MIDIChannels = wxAtoi(AttrStr);

				SynthNode->GetAttribute("progchangemode", &AttrStr);
				if (AttrStr == "gs") Synth->Config.ProgChangeMode = 1;
				else if (AttrStr == "mma") Synth->Config.ProgChangeMode = 2;
				else if (AttrStr == "xg") Synth->Config.ProgChangeMode = 3;
				else Synth->Config.ProgChangeMode = 0;
			}
			else if (SynthNode->GetName() == "network_midi")
			{
				SynthNode->GetAttribute("mode", &AttrStr);
				Synth->Config.NetworkMIDIMode = wxAtoi(AttrStr);

				SynthNode->GetAttribute("localport", &AttrStr);
				Synth->Config.LocalUDPPort = wxAtoi(AttrStr);

				SynthNode->GetAttribute("sessioninitiator", &AttrStr);
				if (AttrStr == "1") Synth->Config.IsSessionInitiator = true;
				else Synth->Config.IsSessionInitiator = false;

				SynthNode->GetAttribute("targetport", &AttrStr);
				Synth->Config.RemoteUDPPort = wxAtoi(AttrStr);

				RemoteIP = SynthNode->GetAttribute("targetip", "127.0.0.1");
				Synth->Config.RemoteDeviceIP = htonl(inet_addr(RemoteIP.mb_str()));
			}

			SynthNode = SynthNode->GetNext();
		} while (SynthNode != 0);
	}
}  // LoadSynthesizer
// -----------------------------------------------------

int CwxFluidSynth::LoadEngineConfiguration(void)
{
	wxXmlDocument ConfigXML;
	wxXmlNode* RootNode;
	wxXmlNode* TopNode;

	// Check first if file exists (otherwise wxWidgets shows an alert box when ConfigXML.Load is called)
	if (wxFileExists("config.cfg") == false) return -1;

	// Try to load the project file as a XML document
	// TODO : on Windows, configuration file could be stored in User/ApplicationData
	if (!ConfigXML.Load("config.cfg"))
	{
		return -2;		// Can not load configuration file
	}

	RootNode = ConfigXML.GetRoot();
	if (RootNode->GetName() != "wxfluidconfig")
	{
		return -3;	// Not a wxFluid configuration file
	}

	// Parse all sections
	TopNode = RootNode->GetChildren();
	if (TopNode != 0)
	{
		do {

			if (TopNode->GetName() == "synthesizer")
			{
				LoadSynthesizer(TopNode, this);
			}

			TopNode = TopNode->GetNext();
		} while (TopNode != 0);
	}

	return 0;
}  // CwxFluidSynth::LoadEngineConfiguration
// -----------------------------------------------------

int CwxFluidSynth::SaveEngineConfiguration(void)
{
	FILE* ConfigFile;
	wxString ParamStr;
	wxString CIPStr;
	struct in_addr ip_addr;

	ConfigFile = fopen("config.cfg", "wt");
	if (ConfigFile == 0) return -1;		// Can not create configuration file

	// Save XML header and create top level tag
	fprintf(ConfigFile, "<?xml version='1.0' encoding='utf-8'?>\n");
	fprintf(ConfigFile, "<wxfluidconfig version=\"1.0\">\n");

	ParamStr = "  <synthesizer>\n";
	fprintf(ConfigFile, ParamStr);

	// Save audio parameters
	ParamStr = "    <audio ";
	ParamStr += "driver=\"" + wxString::Format("%d", this->Config.AudioDriverType) + "\" ";
	ParamStr += "device=\"" + this->Config.AudioDeviceName + "\" ";
	ParamStr += "buffers=\"" + wxString::Format("%d", this->Config.AudioPeriods) + "\" ";
	ParamStr += "buffersize=\"" + wxString::Format("%d", this->Config.AudioBufferSize) + "\" ";
	ParamStr += "samplerate=\"" + wxString::Format("%d", this->Config.SampleRate) + "\" ";

	//  TODO : save samples format
#ifdef __TARGET_WIN__
	if (this->Config.ExclusiveWasapi)
		ParamStr += "exclusivewasapi=\"1\"";
	else
		ParamStr += "exclusivewasapi=\"0\"";
#endif
	ParamStr += "/>\n";
	fprintf(ConfigFile, ParamStr);

	// Save MIDI Parameters
	ParamStr = "    <midi ";
	if (this->Config.UseMIDIInput == false)
		ParamStr += "active=\"0\" ";
	else
		ParamStr += "active=\"1\" ";

	// Driver type is not saved on Windows platform for now (only WinMME)
	ParamStr += "device=\"" + this->Config.MIDIInputName + "\" ";
	ParamStr += "channels=\"" + wxString::Format("%d", this->Config.MIDIChannels) + "\" ";
	switch (this->Config.ProgChangeMode)
	{
	case 1:
		ParamStr += "progchangemode=\"gs\" ";
		break;
	case 2:
		ParamStr += "progchangemode=\"mma\" ";
		break;
	case 3:
		ParamStr += "progchangemode=\"xg\" ";
		break;
	default:
		ParamStr += "progchangemode=\"gm\" ";
	}
	ParamStr += "/>\n";
	fprintf(ConfigFile, ParamStr);

	// Save Network MIDI parameters
	ip_addr.s_addr = htonl(this->Config.RemoteDeviceIP);
	CIPStr = inet_ntoa(ip_addr);

	ParamStr = "    <network_midi mode=\"" + wxString::Format("%d", this->Config.NetworkMIDIMode) + "\" localport=\"" + wxString::Format("%d", this->Config.LocalUDPPort) + "\"";
	ParamStr += " sessioninitiator=\"" + wxString::Format("%d", this->Config.IsSessionInitiator) + "\"";
	ParamStr += " targetip=\"" + CIPStr + "\"";
	ParamStr += " targetport=\"" + wxString::Format("%d", this->Config.RemoteUDPPort) + "\"";
	ParamStr += "/>\n";
	fprintf(ConfigFile, ParamStr);

	fprintf(ConfigFile, "  </synthesizer>\n");

	fprintf(ConfigFile, "</wxfluidconfig>\n");
	fclose(ConfigFile);

	return 0;
}  // CwxFluidSynth::SaveEngineConfiguration
//-----------------------------------------------------------------------------
