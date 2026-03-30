/*
 *  PresetConfigFile.cpp
 *  wxFluid Synthesizer
 *  Methods to save and load wxFluid presets of CwxFluidSynth class
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

#include <stdio.h>
#include <wx/xml/xml.h>
#include "SynthClass.h"

//! Replace XML reserved chars in string by escape sequence
// & is replaced by &amp;
// ' is replaced by &apos;
// " is replaced by &quot;
// < is replaced by &lt;
// > is replaced by &gt;
static void ReplaceXMLChars(wxString& InString, wxString* OutString)
{
	wxUniChar InChar;

	*OutString = "";
	if (InString.Length() == 0) return;

	for (unsigned int CharCount = 0; CharCount < InString.Length(); CharCount++)
	{
		InChar = InString.GetChar(CharCount);
		if (InChar == '&')
			*OutString += "&amp;";
		else if (InChar == '"')
			*OutString += "&quot;";
		else if (InChar == '<')
			*OutString += "&lt;";
		else if (InChar == '>')
			*OutString += "&gt;";
		else
			*OutString += InChar;
	}
}  // ReplaceXMLChars
// -----------------------------------------------------

int CwxFluidSynth::SavePresetConfiguration(wxString PresetFileName)
{
	FILE* ConfigFile;
	wxString ParamStr;
	int FontCounter;
	int ChannelCounter;
	wxString XMLStr;

	ConfigFile = fopen(PresetFileName, "wt");
	if (ConfigFile == 0) return -1;		// Can not create configuration file

	// Save XML header and create top level tag
	fprintf(ConfigFile, "<?xml version='1.0' encoding='utf-8'?>\n");
	fprintf(ConfigFile, "<wxfluidpreset version=\"1.0\">\n");

	// Save soundfont names
	fprintf(ConfigFile, "  <soundfonts>\n");

	for (FontCounter = 0; FontCounter < MAX_SOUND_FONTS; FontCounter++)
	{
		if (this->SoundFontFilePath[FontCounter] != "")
		{
			ReplaceXMLChars(this->SoundFontFilePath[FontCounter], &XMLStr);

			ParamStr = "    <soundfont ";
			ParamStr += "slot=\"" + wxString::Format("%d", FontCounter) + "\" ";
			ParamStr += "file=\"" + XMLStr + "\" ";
			ParamStr += "/>\n";
			fprintf(ConfigFile, ParamStr);
		}
	}

	fprintf(ConfigFile, "  </soundfonts>\n");

	// Save MIDI presets
	fprintf(ConfigFile, "  <presets>\n");
	for (ChannelCounter = 0; ChannelCounter < 16; ChannelCounter++)
	{
		if (this->SoundFontSlotForMIDIChannel[ChannelCounter] != -1)
		{  // Sound defined for this channel
			ParamStr = "    <preset channel=\"" + wxString::Format("%d", ChannelCounter) + "\" ";
			ParamStr += "soundfont=\"" + wxString::Format("%d", this->SoundFontSlotForMIDIChannel[ChannelCounter]) + "\" ";
			ParamStr += "bank=\"" + wxString::Format("%d", this->PresetBank[ChannelCounter]) + "\" ";
			ParamStr += "program=\"" + wxString::Format("%d", this->ProgNumber[ChannelCounter]) + "\" ";
			ParamStr += "/>\n";
			fprintf(ConfigFile, ParamStr);
		}
	}
	fprintf(ConfigFile, "  </presets>\n");

	// Save front panel data
	fprintf(ConfigFile, "  <frontpanel>\n");
	fprintf(ConfigFile, "    <main volume=\"" + wxString::Format("%1.3f", this->VolumeControl) + "\"/>\n");
	ParamStr = "    <reverb";
	ParamStr += " state=\"" + wxString::Format("%d", this->ReverbControlOn) + "\"";
	ParamStr += " size=\"" + wxString::Format("%1.3f", this->ReverbRoomSizeControl) + "\"";
	ParamStr += " damping=\"" + wxString::Format("%1.3f", this->ReverbDampingControl) + "\"";
	ParamStr += " width=\"" + wxString::Format("%1.3f", this->ReverbWidthControl) + "\"";
	ParamStr += " level=\"" + wxString::Format("%1.3f", this->ReverbLevelControl) + "\"";
	ParamStr += " />\n";
	fprintf(ConfigFile, ParamStr);

	ParamStr = "    <chorus";
	ParamStr += " state=\"" + wxString::Format("%d", this->ChorusControlOn) + "\"";
	ParamStr += " lines=\"" + wxString::Format("%d", this->ChorusNumberControl) + "\"";
	ParamStr += " speed=\"" + wxString::Format("%1.3f", this->ChorusSpeedControl) + "\"";
	ParamStr += " depth=\"" + wxString::Format("%1.3f", this->ChorusDepthControl) + "\"";
	ParamStr += " level=\"" + wxString::Format("%1.3f", this->ChorusLevelControl) + "\"";
	ParamStr += " wave=\"" + wxString::Format("%d", this->ChorusWaveTriangle) + "\"";
	ParamStr += " />\n";
	fprintf(ConfigFile, ParamStr);

	fprintf(ConfigFile, "  </frontpanel>\n");

	fprintf(ConfigFile, "</wxfluidpreset>\n");
	fclose(ConfigFile);

	this->PresetConfigurationChanged = false;

	return 0;
}  // CwxFluidSynth::SavePresetConfiguration
// -----------------------------------------------------

static void LoadSoundFontList(wxXmlNode* ParentNode, CwxFluidSynth* Synth)
{
	wxXmlNode* SFNode;
	wxString AttrStr;
	int Slot;

	SFNode = ParentNode->GetChildren();
	if (SFNode != 0)
	{
		do
		{
			if (SFNode->GetName() == "soundfont")
			{
				SFNode->GetAttribute("slot", &AttrStr);
				Slot = wxAtoi(AttrStr);
				if ((Slot >= 0) && (Slot < MAX_SOUND_FONTS))
				{
					SFNode->GetAttribute("file", &AttrStr);
					Synth->SoundFontFilePath[Slot] = AttrStr;
				}
			}

			SFNode = SFNode->GetNext();
		} while (SFNode != 0);
	}
}  // LoadSoundFontList
// -----------------------------------------------------

static void LoadPresets(wxXmlNode* ParentNode, CwxFluidSynth* Synth)
{
	wxXmlNode* PresetNode;
	wxString AttrStr;
	int Channel;
	int SFIndex;
	int Bank;
	int Program;

	PresetNode = ParentNode->GetChildren();
	if (PresetNode != 0)
	{
		do
		{
			if (PresetNode->GetName() == "preset")
			{
				PresetNode->GetAttribute("channel", &AttrStr);
				Channel = wxAtoi(AttrStr);
				if ((Channel >= 0) && (Channel < 16))
				{
					PresetNode->GetAttribute("soundfont", &AttrStr);
					SFIndex = wxAtoi(AttrStr);
					if ((SFIndex >= 0) && (SFIndex < MAX_SOUND_FONTS))
					{
						PresetNode->GetAttribute("bank", &AttrStr);
						Bank = wxAtoi(AttrStr);
						if ((Bank >= 0) && (Bank <= 16383))
						{
							PresetNode->GetAttribute("program", &AttrStr);
							Program = wxAtoi(AttrStr);

							if ((Program >= 0) && (Program < 128))
							{
								Synth->SoundFontSlotForMIDIChannel[Channel] = SFIndex;
								Synth->PresetBank[Channel] = Bank;
								Synth->ProgNumber[Channel] = Program;
							}
						}
					}
				}
			}
			PresetNode = PresetNode->GetNext();
		} while (PresetNode != 0);
	}
}  // LoadPresets
// -----------------------------------------------------

static void ClipParameter(float* Parameter)
{
	if (*Parameter > 1.0f)
		*Parameter = 1.0f;
	if (*Parameter < 0.0f)
		*Parameter = 0.0f;
}  // ClipParameter
// -----------------------------------------------------

static void LoadPresetFrontPanel(wxXmlNode* SynthesizerNode, CwxFluidSynth* Synth)
{
	wxXmlNode* ParameterNode;
	wxString ParamStr;

	ParameterNode = SynthesizerNode->GetChildren();
	if (ParameterNode != 0)
	{
		do
		{
			if (ParameterNode->GetName() == "main")
			{
				ParameterNode->GetAttribute("volume", &ParamStr);
				Synth->VolumeControl = wxAtof(ParamStr);
				ClipParameter(&Synth->VolumeControl);
			}
			else if (ParameterNode->GetName() == "reverb")
			{
				ParameterNode->GetAttribute("state", &ParamStr);
				if (ParamStr == "1") Synth->ReverbControlOn = true;
				else Synth->ReverbControlOn = false;

				ParameterNode->GetAttribute("size", &ParamStr);
				Synth->ReverbRoomSizeControl = wxAtof(ParamStr);
				ClipParameter(&Synth->ReverbRoomSizeControl);

				ParameterNode->GetAttribute("damping", &ParamStr);
				Synth->ReverbDampingControl = wxAtof(ParamStr);
				ClipParameter(&Synth->ReverbDampingControl);

				ParameterNode->GetAttribute("width", &ParamStr);
				Synth->ReverbWidthControl = wxAtof(ParamStr);
				ClipParameter(&Synth->ReverbWidthControl);

				ParameterNode->GetAttribute("level", &ParamStr);
				Synth->ReverbLevelControl = wxAtof(ParamStr);
				ClipParameter(&Synth->ReverbLevelControl);
			}
			else if (ParameterNode->GetName() == "chorus")
			{
				ParameterNode->GetAttribute("state", &ParamStr);
				if (ParamStr == "1") Synth->ChorusControlOn = true;
				else Synth->ChorusControlOn = false;

				ParameterNode->GetAttribute("lines", &ParamStr);
				Synth->ChorusNumberControl = wxAtoi(ParamStr);

				ParameterNode->GetAttribute("speed", &ParamStr);
				Synth->ChorusSpeedControl = wxAtof(ParamStr);
				ClipParameter(&Synth->ChorusSpeedControl);

				ParameterNode->GetAttribute("depth", &ParamStr);
				Synth->ChorusDepthControl = wxAtof(ParamStr);
				ClipParameter(&Synth->ChorusDepthControl);

				ParameterNode->GetAttribute("level", &ParamStr);
				Synth->ChorusLevelControl = wxAtof(ParamStr);
				ClipParameter(&Synth->ChorusLevelControl);

				ParameterNode->GetAttribute("wave", &ParamStr);
				if (ParamStr == "1") Synth->ChorusWaveTriangle = true;
				else Synth->ChorusWaveTriangle = false;
			}
			ParameterNode = ParameterNode->GetNext();
		} while (ParameterNode != 0);
	}
}  // LoadPresetFrontPanel
// -----------------------------------------------------

int CwxFluidSynth::LoadPresetConfiguration(wxString& PresetFileName)
{
	wxXmlDocument ConfigXML;
	wxXmlNode* RootNode;
	wxXmlNode* TopNode;

	// Check first if file exists (otherwise wxWidgets shows an alert box when ConfigXML.Load is called)
	if (wxFileExists(PresetFileName) == false) return -1;

	// Try to load the project file as a XML document
	if (!ConfigXML.Load(PresetFileName))
	{
		return -2;		// Can not load configuration file
	}

	RootNode = ConfigXML.GetRoot();
	if (RootNode->GetName() != "wxfluidpreset")
	{
		return -3;	// Not a wxFluid preset file
	}

	// Parse all sections
	TopNode = RootNode->GetChildren();
	if (TopNode != 0)
	{
		do {

			if (TopNode->GetName() == "soundfonts")
			{
				LoadSoundFontList(TopNode, this);
			}
			else if (TopNode->GetName() == "presets")
			{
				LoadPresets(TopNode, this);
			}
			else if (TopNode->GetName() == "frontpanel")
			{
				LoadPresetFrontPanel(TopNode, this);
			}

			TopNode = TopNode->GetNext();
		} while (TopNode != 0);
	}

	return 0;
}  // LoadPresetConfiguration
// -----------------------------------------------------
