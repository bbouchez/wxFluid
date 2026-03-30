/*
 *  SynthClass.cpp
 *  wxFluid Synthesizer
 *  Definition of CwxFluidSynth class
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

#include "SynthClass.h"
#include "UMP_Transcoder.h"

#define MAX_OUTPUT_GAIN		3.0f

//! MIDI IN callback for RtMidi instances
void RtMidiCallback(double deltatime, std::vector< unsigned char > *message, void *userData)
{
	CwxFluidSynth* Synth = (CwxFluidSynth*)userData;

	Synth->RTMIDICallback(message);
}  // RtMidiCallback
//-----------------------------------------------------------------------------

#ifdef __TARGET_MAC__
// This callback is called from realtime thread. Processing time in the callback shall be kept to a minimum
void RTPMIDIDataCallback (void* UserInstance, unsigned int DataSize, unsigned char* DataBlock, unsigned int DeltaTime)
#endif

#ifdef __TARGET_LINUX__
void RTPMIDIDataCallback (void* UserInstance, unsigned int DataSize, unsigned char* DataBlock, unsigned int DeltaTime)
#endif

#ifdef __TARGET_WIN__
void CALLBACK RTPMIDIDataCallback (void* UserInstance, unsigned int DataSize, unsigned char* DataBlock, unsigned int DeltaTime)
#endif
{
    CwxFluidSynth* Synth = (CwxFluidSynth*)UserInstance;

    Synth->ProcessRTPMIDI(DataSize, DataBlock);
}  // RTPMIDIDataCallback
//-----------------------------------------------------------------------------

#ifdef __TARGET_MAC__
// This callback is called from realtime thread. Processing time in the callback shall be kept to a minimum
void UMPDataCallback (void* UserInstance, uint32_t* DataBlock)
#endif

#ifdef __TARGET_LINUX__
void UMPDataCallback (void* UserInstance, uint32_t* DataBlock)
#endif

#ifdef __TARGET_WIN__
void CALLBACK UMPDataCallback(void* UserInstance, uint32_t* DataBlock)
#endif
{
    CwxFluidSynth* Synth = (CwxFluidSynth*)UserInstance;

    Synth->ProcessNetUMP(DataBlock);
}  // UMPDataCallback
//-----------------------------------------------------------------------------

void* NetMIDIThreadFunc(CThread* Control)
{
	CwxFluidSynth* Synth = (CwxFluidSynth*)Control->UserData;

	while (Control->ShouldStop == false)
	{
		Synth->RunNetworkMIDI();
		SystemSleepMillis(1);
	}  // ShouldStop = false
	Control->IsStopped = true;
	//pthread_exit(NULL);
	return 0;
}  // NetMIDIThreadFunc
//-----------------------------------------------------------------------------

CwxFluidSynth::CwxFluidSynth(void)
{
	this->settings = 0;
	this->synth = 0;
	this->adriver = 0;
	this->MIDIInput = 0;
	this->PresetConfigurationChanged = false;
	this->RTPMIDIHandler = 0;
	this->NetUMPHandler = 0;
	this->NetMIDIThread = 0;

	for (int i = 0; i < MAX_SOUND_FONTS; i++)
	{
		this->FontID[i] = -1;
		this->SoundFontFilePath[i] = "";
	}

	for (int i = 0; i < 16; i++)
	{
		this->SoundFontSlotForMIDIChannel[i] = -1;		// No soundfont associated to the MIDI channel by default
		this->PresetBank[i] = 0;
		this->ProgNumber[i] = 0;
	}

	this->Config.AudioDriverType = 0;
#ifdef __TARGET_MAC__
    this->Config.AudioDeviceName = "default";
#else
	this->Config.AudioDeviceName = "";
#endif
	this->Config.ExclusiveWasapi = 0;
	this->Config.AudioPeriods = 8;
	this->Config.AudioBufferSize = 256;
	this->Config.SampleRate = 44100;

	this->Config.UseMIDIInput = false;
	this->Config.MIDIInputName = "";
	this->Config.MIDIChannels = 16;
	this->Config.ProgChangeMode = 0;
	this->Config.IsSessionInitiator = false;
	this->Config.NetworkMIDIMode = MIDI_NETWORK_OFF;
	this->Config.RemoteDeviceIP = 0x7F000001;
	this->Config.LocalUDPPort = 6004;
	this->Config.RemoteUDPPort = 5004;

	this->Config.SynthStereoPairs = 1;
	this->Config.SynthAudioGroups = 1;
	this->Config.SynthMaxPolyphony = 256;

    this->VolumeControl = DEFAULT_VOLUME;
    this->ReverbControlOn = false;
    this->ReverbRoomSizeControl = DEFAULT_REVERB_ROOM_SIZE;
    this->ReverbDampingControl = DEFAULT_REVERB_DAMPING;
    this->ReverbWidthControl = DEFAULT_REVERB_WIDTH;
    this->ReverbLevelControl = DEFAULT_REVERB_LEVEL;
    this->ChorusControlOn = false;
    this->ChorusNumberControl = DEFAULT_CHORUS_NUMBER;
    this->ChorusSpeedControl = DEFAULT_CHORUS_SPEED;
    this->ChorusDepthControl = DEFAULT_CHORUS_DEPTH;
    this->ChorusLevelControl = DEFAULT_CHORUS_LEVEL;
	this->ChorusWaveTriangle = false;
}  // CwxFluidSynth::CFluidSynth
// -----------------------------------------------------

CwxFluidSynth::~CwxFluidSynth(void)
{
	this->StopSynth();
}  // CwxFluidSynth::~CwxFluidSynth
// -----------------------------------------------------

int CwxFluidSynth::CreateFluidEngine(void)
{
	int Res;

	this->settings = new_fluid_settings();
	if (this->settings == 0) return -1;		// Can not create settings

	Res = FLUID_FAILED;
#ifdef __TARGET_WIN__
	switch (this->Config.AudioDriverType)
	{
		case WINDOWS_MME_DRIVER :
			Res = fluid_settings_setstr(this->settings, "audio.driver", "waveout");
			Res = fluid_settings_setstr(this->settings, "audio.waveout.device", this->Config.AudioDeviceName);
			if (Res == FLUID_FAILED)
				wxMessageBox("Audio device " + this->Config.AudioDeviceName + " can not be opened", "MME Waveout error", wxOK + wxICON_ERROR);
			break;
		case WINDOWS_DIRECTSOUND_DRIVER :
			Res = fluid_settings_setstr(this->settings, "audio.driver", "dsound");
			Res = fluid_settings_setstr(this->settings, "audio.dsound.device", this->Config.AudioDeviceName);
			if (Res == FLUID_FAILED)
				wxMessageBox("Audio device " + this->Config.AudioDeviceName + " can not be opened", "DirectSound error", wxOK + wxICON_ERROR);
			break;
		case WINDOWS_WASAPI_DRIVER :
			Res = fluid_settings_setstr(this->settings, "audio.driver", "wasapi");
			Res = fluid_settings_setstr(this->settings, "audio.wasapi.device", this->Config.AudioDeviceName);
			if (Res == FLUID_FAILED)
				wxMessageBox("Audio device " + this->Config.AudioDeviceName + " can not be opened", "WASAPI error", wxOK + wxICON_ERROR);
			Res = fluid_settings_setint(this->settings, "audio.wasapi.exclusive-mode", this->Config.ExclusiveWasapi);
			break;
	}
	//Res = fluid_settings_setstr(this->settings, "audio.driver", "jack");
#endif
#ifdef __TARGET_LINUX__
    switch (this->Config.AudioDriverType)
    {
        case LINUX_JACK_DRIVER :
            Res = fluid_settings_setstr(this->settings, "audio.driver", "jack");
            //Res = fluid_settings_setstr(this->settings, "audio.jack.autoconnect", "1");
            break;
        case LINUX_ALSA_DRIVER :
            Res = fluid_settings_setstr(this->settings, "audio.driver", "alsa");
			Res = fluid_settings_setstr(this->settings, "audio.alsa.device", this->Config.AudioDeviceName);
			break;
    }
#endif
#ifdef __TARGET_MAC__
    Res = fluid_settings_setstr(this->settings, "audio.driver", "coreaudio");
    Res = fluid_settings_setstr(this->settings, "audio.coreaudio.device", this->Config.AudioDeviceName);
#endif

	Res = fluid_settings_setint(this->settings, "audio.periods", this->Config.AudioPeriods);
	Res = fluid_settings_setint(this->settings, "audio.period-size", this->Config.AudioBufferSize);
	Res = fluid_settings_setint(this->settings, "audio.realtime-prio", 99);
	Res = fluid_settings_setnum(this->settings, "synth.sample-rate", this->Config.SampleRate);

	this->synth = new_fluid_synth(this->settings);
	this->adriver = new_fluid_audio_driver(this->settings, this->synth);

	this->StartMIDI();

#ifdef __TARGET_LINUX__
	// Connect automatically Fluidsynth to JACK system outputs
    if (this->Config.AudioDriverType==LINUX_JACK_DRIVER)
    {
        wxShell("jack_connect fluidsynth:left system:playback_1");
        wxShell("jack_connect fluidsynth:right system:playback_2");
    }
#endif // __TARGET_LINUX__

	return 0;
}  // CwxFluidSynth::CreateSynth
// -----------------------------------------------------

void CwxFluidSynth::StopSynth(void)
{
#ifdef __TARGET_LINUX__
	// Removed as this call leads to a segmentation fault
	if (this->Config.AudioDriverType == LINUX_JACK_DRIVER)
	{
		wxShell("jack_disconnect fluidsynth:left system:playback_1");
		wxShell("jack_disconnect fluidsynth:right system:playback_2");
	}
#endif // __TARGET_LINUX__

	this->StopMIDI();

	for (int SFNum = 0; SFNum < MAX_SOUND_FONTS; SFNum++)
	{
		UnloadSoundFont(SFNum);
	}

	delete_fluid_audio_driver(this->adriver);
	this->adriver = 0;
	delete_fluid_synth(this->synth);
	this->synth = 0;
	delete_fluid_settings(this->settings);
	this->settings = 0;
}  // CwxFluidSynth::StopSynth
// -----------------------------------------------------

void CwxFluidSynth::RTMIDICallback(std::vector< unsigned char > *message)
{
    this->SendMIDI1ToFluid(&message->at(0));
}  // CwxFluidSynth::RTMIDICallback
// -----------------------------------------------------

void CwxFluidSynth::ProcessRTPMIDI(unsigned int DataSize, unsigned char* DataBlock)
{
    if (DataSize<=3)
    {
        this->SendMIDI1ToFluid(DataBlock);
    }
}  // CwxFluidSynth::ProcessRTPMIDI
// -----------------------------------------------------

void CwxFluidSynth::ProcessNetUMP(uint32_t* DataBlock)
{
    uint8_t MIDI1Msg[3];        // Transcoder gives a maximum message size of 3 bytes
    unsigned int MIDI1Size;
    unsigned int Filter;
    uint32_t UMPReply[4];
    uint32_t Status;
    uint32_t Channel;
    uint32_t Note;
    uint32_t Velocity;
    uint32_t Control;
    uint32_t Value;
    uint32_t BankMSB;
    uint32_t BankLSB;
	//unsigned int BendValue;
    int SFID;

    // Process UMP Discovery messages
    if ((DataBlock[0]&0xFFFF0000)==0xF0000000)
    {
        Filter = DataBlock[1]&0xF;

        if (Filter&0x01)
        {  // e bit set : request Endpoint Info Notification
            UMPReply[0] = 0xF0010101;       // Endpoint Info Notification, V1.1
            UMPReply[1] = 0x00000300;       // Static Function Blocks, no Function Blocks, support MIDI 1.0 and MIDI 2.0, don't support transmit and receive JR
            UMPReply[2] = 0x00000000;       // Reserved
            UMPReply[3] = 0x00000000;       // Reserved

            NetUMPHandler->SendUMPMessage(&UMPReply[0]);
        }

        if (Filter&0x02)
        {  // d bit set : request Device Identity Notification
            UMPReply[0] = 0xF0020000;       // Device Identity Notification
            UMPReply[1] = 0x0000207C;       // KissBox B.V. SYSEX ID (as there is no "Prototype" ID for now
            UMPReply[2] = 0x02000500;       // Device family (Software = 0x02 / Model = wxFluid
			UMPReply[3] = (WXFLUID_VERSION << 24) + (WXFLUID_REVISION << 16) + (WXFLUID_SUBREVISION << 8);

            NetUMPHandler->SendUMPMessage(&UMPReply[0]);
        }

        if (Filter&0x04)
        {  // n bit set : request Endpoint Name Notification
            UMPReply[0] = 0xF4037778;       // Endpoint Name, wx        (Start UMP)
            UMPReply[1] = 0x466C7569;       // Flui
            UMPReply[2] = 0x64205346;       // d SF
            UMPReply[3] = 0x32205379;       // 2 Sy

            NetUMPHandler->SendUMPMessage(&UMPReply[0]);

            UMPReply[0] = 0xFC036E74;       // nt        (End UMP)
            UMPReply[1] = 0x68657369;       // hesi
            UMPReply[2] = 0x7A657200;       // zer
            UMPReply[3] = 0x00000000;       //

            NetUMPHandler->SendUMPMessage(&UMPReply[0]);
        }

        if (Filter&0x08)
        {  // i bit set : request Product Instance ID Notification
            // TODO : generate a Product Instance ID
        }

        if (Filter&0x10)
        {  // s bit set : request Stream Configuration notification
            UMPReply[0] = 0xF0060300;       // Stream Configuration Notification, MIDI 1.0 and MIDI 2.0 protocols, no TX/RX jitter reduction
            UMPReply[1] = 0x00000000;       // Reserved
            UMPReply[2] = 0x00000000;       // Reserved
            UMPReply[3] = 0x00000000;       // Reserved

            NetUMPHandler->SendUMPMessage(&UMPReply[0]);
        }
    }

    MIDI1Size = TranscodeUMP_MIDI1 (DataBlock, &MIDI1Msg[0]);
    if (MIDI1Size>0)
    {  // Message transcoded successfully as MIDI 1.0
        this->SendMIDI1ToFluid(&MIDI1Msg[0]);
    }
    else if ((DataBlock[0]&0xF0000000)==0x40000000)
    {  // Decode MIDI 2.0 messages
        Status = (DataBlock[0]>>20)&0x0F;
        Channel = (DataBlock[0]>>16)&0x0F;
        switch (Status)
        {
            case 0x8 :  // Note Off
                Note = (DataBlock[0]>>8)&0x7F;
                fluid_synth_noteoff(this->synth, Channel, Note);
                break;
            case 0x9 :  // Note On
                Note = (DataBlock[0]>>8)&0x7F;
                Velocity = DataBlock[1]>>25;
                if (Velocity==0)
                    Velocity=1;     // Fluidsynth does not accept velocity = 0 for a Note On
                fluid_synth_noteon(this->synth, Channel, Note, Velocity);
                break;
            case 0xA :  // Key Pressure
                Note = (DataBlock[0]>>8)&0x7F;
                Value = DataBlock[1]>>25;
                fluid_synth_key_pressure(this->synth, Channel, Note, Value);
                break;
            case 0xB :  // Control Change
                Control = (DataBlock[0]>>8)&0x7F;
                Value = DataBlock[1]>>25;
                fluid_synth_cc(this->synth, Channel, Control, Value);
                break;
            case 0xC :  // Program Change
                // Check if B bit is set
                if ((DataBlock[0]&1)==0)
                {  // Bit B not set : perform only a Program Change
                    //fluid_synth_program_change(this->synth, Channel, DataBlock[1]>>24);
                    // Make sure a SoundFont is loaded in the channel before we call programm_select (otherwise FluidSynth can raise an exception)
                    SFID = this->FontID[Channel];
                    if (SFID!=-1)
                    {
                        fluid_synth_program_select(this->synth, Channel, SFID, this->PresetBank[Channel], DataBlock[1]>>24);
                    }
                }
                else
                {  // Use bank and preset information
                    BankLSB = DataBlock[1]&0x7F;
                    BankMSB = (DataBlock[1]>>8)&0x7F;
                    // Make sure a SoundFont is loaded in the channel before we call programm_select (otherwise FluidSynth can raise an exception)
                    SFID = this->FontID[Channel];
                    if (SFID!=-1)
                    {
                        fluid_synth_program_select(this->synth, Channel, SFID, (BankMSB<<7)+BankLSB, DataBlock[1]>>24);
                    }
                }
                break;
            case 0xD :  // Channel Pressure
                fluid_synth_channel_pressure(this->synth, Channel, DataBlock[1]>>25);
                break;
            case 0xE :
                fluid_synth_pitch_bend(this->synth, Channel, DataBlock[1]>>18);
                break;
        }
    }
}  // CwxFluidSynth::ProcessNetUMP
// -----------------------------------------------------

void CwxFluidSynth::SendMIDI1ToFluid (uint8_t* MIDIMsg)
{
    uint8_t Status = MIDIMsg[0] & 0xF0;
    uint8_t Channel = MIDIMsg[0] & 0x0F;
    uint8_t Note;
    uint8_t Velocity;
    uint8_t Control;
    uint8_t Value;
    uint8_t Program;
    unsigned int BendValue;
    int SFID;

	if (MIDIMsg[0] == 0xF8) return;

    switch (Status)
    {
    case 0x80 :
        Note = MIDIMsg[1];
        fluid_synth_noteoff(this->synth, Channel, Note);
        break;
    case 0x90:
        Note = MIDIMsg[1];
        Velocity = MIDIMsg[2];
        if (Velocity!=0)
            fluid_synth_noteon(this->synth, Channel, Note, Velocity);
        else
            fluid_synth_noteoff(this->synth, Channel, Note);
        break;
    case 0xA0:
        Note = MIDIMsg[1];
        Value = MIDIMsg[2];
        fluid_synth_key_pressure(this->synth, Channel, Note, Value);
        break;
    case 0xB0:
        Control = MIDIMsg[1];
        Value = MIDIMsg[2];
        fluid_synth_cc(this->synth, Channel, Control, Value);
        break;
    case 0xC0:
        Program = MIDIMsg[1];
        //fluid_synth_program_change(this->synth, Channel, Program);
		if (this->SoundFontSlotForMIDIChannel[Channel] != -1)		// We have a SoundFont associated with this MIDI channel
		{
			SFID = this->FontID[this->SoundFontSlotForMIDIChannel[Channel]];
			if (SFID != -1)
			{
				fluid_synth_program_select(this->synth, Channel, SFID, this->PresetBank[Channel], Program);
			}
		}
        break;
    case 0xD0:
        Value = MIDIMsg[1];
        fluid_synth_channel_pressure(this->synth, Channel, Value);
        break;
    case 0xE0 :
        BendValue = (unsigned int)MIDIMsg[1] + (((unsigned int)MIDIMsg[2]) << 7);
        fluid_synth_pitch_bend(this->synth, Channel, BendValue);
        break;
    }
}  // CwxFluidSynth::SendMIDI1ToFluid
// -----------------------------------------------------

int CwxFluidSynth::LoadSoundFont(wxString& SFPath, unsigned int Position)
{
	if (this->FontID[Position] != -1)
	{  // Already a soundfont in this slot
		return -2;
	}

	this->FontID[Position] = fluid_synth_sfload(this->synth, SFPath, 0);    // What does it mean to "reset presets" ?
	this->SoundFontFilePath[Position] = SFPath;

	return this->FontID[Position];
}  // CwxFluidSynth::LoadSoundFont
// -----------------------------------------------------

void CwxFluidSynth::UnloadSoundFont(unsigned int Position)
{
    if (this->FontID[Position] == -1) return;       // No SoundFont to unload

	fluid_synth_sfunload(this->synth, this->FontID[Position], 1);

	this->FontID[Position] = -1;
	this->SoundFontFilePath[Position] = "";
}  // CwxFluidSynth::UnloadSoundFont
// -----------------------------------------------------

void CwxFluidSynth::SetVolume(float Volume)
{
	this->VolumeControl = Volume;
	fluid_synth_set_gain(this->synth, Volume*MAX_OUTPUT_GAIN);
}  // CwxFluidSynth::SetVolume
// -----------------------------------------------------

void CwxFluidSynth::SetReverbState(bool ControlValue)
{
	this->ReverbControlOn = ControlValue;

#ifdef __TARGET_WIN__
	if (ControlValue == true)
		fluid_synth_reverb_on(this->synth, -1, 1);
	else
		fluid_synth_reverb_on(this->synth, -1, 0);
#endif // __TARGET_WIN__

#ifdef __TARGET_LINUX__
	if (ControlValue == true)
		fluid_synth_set_reverb_on(this->synth, 1);
	else
		fluid_synth_set_reverb_on(this->synth, 0);
#endif // __TARGET_LINUX__

#ifdef __TARGET_MAC__
	if (ControlValue == true)
		fluid_synth_set_reverb_on(this->synth, 1);
	else
		fluid_synth_set_reverb_on(this->synth, 0);
#endif // __TARGET_MAC__
}  // CwxFluidSynth::SetReverbState
// -----------------------------------------------------

void CwxFluidSynth::SetReverbRoomSize(float Size)
{
	this->ReverbRoomSizeControl = Size;
#ifdef __TARGET_LINUX__
    fluid_synth_set_reverb_roomsize(this->synth, Size);
#else
	fluid_synth_set_reverb_group_roomsize(this->synth, -1, Size);		// Room size is 0.0 .. 1.0
#endif // __TARGET_LINUX__
}  // CwxFluidSynth::SetReverbRoomSize
// -----------------------------------------------------

void CwxFluidSynth::SetReverbDamping(float Damping)
{
	this->ReverbDampingControl = Damping;
#ifdef __TARGET_LINUX__
    fluid_synth_set_reverb_damp(this->synth, Damping);
#else
	fluid_synth_set_reverb_group_damp(this->synth, -1, Damping);	// Damping is 0.0 .. 1.0
#endif
}  // CwxFluidSynth::SetReverbDamping
// -----------------------------------------------------

void CwxFluidSynth::SetReverbWidth(float Width)
{
	this->ReverbWidthControl = Width;
#ifdef __TARGET_LINUX__
    fluid_synth_set_reverb_width(this->synth, Width*100.0f);
#else
	fluid_synth_set_reverb_group_width(this->synth, -1, Width*100.0f);	// Width is 0.0 .. 100.0
#endif // __TARGET_LINUX__
}  // CwxFluidSynth::SetReverbWidth
// -----------------------------------------------------

void CwxFluidSynth::SetReverbLevel(float Level)
{
	this->ReverbLevelControl = Level;
#ifdef __TARGET_LINUX__
    fluid_synth_set_reverb_level(this->synth, Level);
#else
	fluid_synth_set_reverb_group_level(this->synth, -1, Level);		// Reverb level is 0.0 .. 1.0
#endif // __TARGET_LINUX__
}  // CwxFluidSynth::SetReverbLevel
// -----------------------------------------------------

void CwxFluidSynth::SetChorusState(bool ControlValue)
{
	this->ChorusControlOn = ControlValue;
#ifdef __TARGET_WIN__
	if (ControlValue == true)
		fluid_synth_chorus_on(this->synth, -1, 1);
	else
		fluid_synth_chorus_on(this->synth, -1, 0);
#endif

#ifdef __TARGET_LINUX__
	if (ControlValue == true)
		fluid_synth_set_chorus_on(this->synth, 1);
	else
		fluid_synth_set_chorus_on(this->synth, 0);
#endif // __TARGET_LINUX__

#ifdef __TARGET_MAC__
	if (ControlValue == true)
		fluid_synth_set_chorus_on(this->synth, 1);
	else
		fluid_synth_set_chorus_on(this->synth, 0);
#endif // __TARGET_MAC__
}  // CwxFluidSynth::SetChorusState
// -----------------------------------------------------

void CwxFluidSynth::SetChorusNumber(unsigned int Number)
{
	this->ChorusNumberControl = Number;
#ifdef __TARGET_LINUX__
    fluid_synth_set_chorus_nr(this->synth, Number);
#else
	fluid_synth_set_chorus_group_nr(this->synth, -1, Number);
#endif // __TARGET_LINUX__
}  // CwxFluidSynth::SetChorusNumber
// -----------------------------------------------------

void CwxFluidSynth::SetChorusSpeed(float Speed)
{
	this->ChorusSpeedControl = Speed;

	Speed = Speed * 4.9f;
	Speed = Speed + 0.1f;		// Chorus level is 0.1 .. 5.0
#ifdef __TARGET_LINUX__
    fluid_synth_set_chorus_speed(this->synth, Speed);
#else
	fluid_synth_set_chorus_group_speed(this->synth, -1, Speed);
#endif // __TARGET_LINUX__
}  // CwxFluidSynth::SetChorusSpeed
// -----------------------------------------------------

void CwxFluidSynth::SetChorusDepth(float Depth)
{
	this->ChorusDepthControl = Depth;
#ifdef __TARGET_LINUX__
    fluid_synth_set_chorus_depth(this->synth, Depth*21.0f);
#else
	fluid_synth_set_chorus_group_depth(this->synth, -1, Depth*21.0f);    // Chorus depth is "safe" up to 21.0 for fs=96kHz (said by API reference...)
#endif // __TARGET_LINUX__
}  // CwxFluidSynth::SetChorusDepth
// -----------------------------------------------------

void CwxFluidSynth::SetChorusLevel(float Level)
{
	this->ChorusLevelControl = Level;
#ifdef __TARGET_LINUX__
    fluid_synth_set_chorus_level(this->synth, Level*10.0f);
#else
	fluid_synth_set_chorus_group_level(this->synth, -1, Level*10.0f);  // Chorus level is 0.0 .. 10.0
#endif // __TARGET_LINUX__
}  // CwxFluidSynth::SetChorusLevel
// -----------------------------------------------------

void CwxFluidSynth::SetChorusWave(bool IsTriangle)
{
	this->ChorusWaveTriangle = IsTriangle;
	if (IsTriangle)
	{
#ifdef __TARGET_LINUX__
        fluid_synth_set_chorus_type(this->synth, FLUID_CHORUS_MOD_TRIANGLE);
#else
		fluid_synth_set_chorus_group_type(this->synth, -1, FLUID_CHORUS_MOD_TRIANGLE);
#endif // __TARGET_LINUX__
	}
	else
	{
#ifdef __TARGET_LINUX__
        fluid_synth_set_chorus_type(this->synth, FLUID_CHORUS_MOD_SINE);
#else
		fluid_synth_set_chorus_group_type(this->synth, -1, FLUID_CHORUS_MOD_SINE);
#endif // __TARGET_LINUX__
	}
}  // CwxFluidSynth::SetChorusWave
// -----------------------------------------------------

void CwxFluidSynth::RunNetworkMIDI(void)
{
	if (RTPMIDIHandler)
		RTPMIDIHandler->RunSession();
	if (NetUMPHandler)
		NetUMPHandler->RunSession();
}  // CwxFluidSynth::RunNetworkMIDI
// -----------------------------------------------------

void CwxFluidSynth::ClearPresets(void)
{
	// We ask first to unload the soundfonts. FluidSynth does it "lazy", so a SoudnFont is only unloaded
	// when all channels using the SoundFont stop playing. This "marks" the SoundFont as ready to be unloaded
	for (int SFNum = 0; SFNum < MAX_SOUND_FONTS; SFNum++)
	{
        UnloadSoundFont(SFNum);
	}

	// Now that all SoundFonts are marked for unloading, we force all generators to stop so the SoundFonts will be unloaded
	// even if they were still being used by a channel
	for (int Channel = 0; Channel < 16; Channel++)
	{
		fluid_synth_all_sounds_off(this->synth, Channel);
		this->SoundFontSlotForMIDIChannel[Channel] = -1;
	}

	this->VolumeControl = DEFAULT_VOLUME;
	this->ReverbControlOn = false;
	this->ReverbRoomSizeControl = DEFAULT_REVERB_ROOM_SIZE;
	this->ReverbDampingControl = DEFAULT_REVERB_DAMPING;
	this->ReverbWidthControl = DEFAULT_REVERB_WIDTH;
	this->ReverbLevelControl = DEFAULT_REVERB_LEVEL;
	this->ChorusControlOn = false;
	this->ChorusNumberControl = DEFAULT_CHORUS_NUMBER;
	this->ChorusSpeedControl = DEFAULT_CHORUS_SPEED;
	this->ChorusDepthControl = DEFAULT_CHORUS_DEPTH;
	this->ChorusLevelControl = DEFAULT_CHORUS_LEVEL;
	this->ChorusWaveTriangle = false;
}  // CwxFluidSynth::ClearPresets
// -----------------------------------------------------

void CwxFluidSynth::LoadPresetFile(wxString PresetFileName)
{
	int Slot;
    int Channel;
	int SFID;

	this->ClearPresets();		// To be sure all is clean before we continue
	this->LoadPresetConfiguration(PresetFileName);

	// Loop over all sound fonts and load them into engine (LoadPresetConfiguration only init the file names)
	for (Slot = 0; Slot < MAX_SOUND_FONTS; Slot++)
	{
		if (this->SoundFontFilePath[Slot] != "")
		{
			SFID = this->LoadSoundFont(this->SoundFontFilePath[Slot], Slot);
			if (SFID == -1)
			{
				wxMessageBox("Can not load SoundFont " + this->SoundFontFilePath[Slot], "Error", wxOK + wxICON_ERROR);
			}
		}
	}

	// Once soundfonts are loaded, we can now send Program Change to all channels
	for (Channel = 0; Channel < 16; Channel++)
	{
        // Set each channel in OMNI OFF / POLY ON
        //fluid_synth_set_basic_channel(this->synth, Slot, FLUID_CHANNEL_MODE_OMNIOFF_POLY, 16);

		if (this->SoundFontSlotForMIDIChannel[Channel] != -1)
		{
			SFID = this->FontID[this->SoundFontSlotForMIDIChannel[Channel]];
			if (SFID != -1)
			{
				fluid_synth_program_select(this->synth, Channel, SFID, this->PresetBank[Channel], this->ProgNumber[Channel]);
			}
		}
	}

	// Send control value from front panel stored values
	this->SetVolume(this->VolumeControl);
	this->SetReverbState(this->ReverbControlOn);
	this->SetReverbRoomSize(this->ReverbRoomSizeControl);
	this->SetReverbDamping(this->ReverbDampingControl);
	this->SetReverbWidth(this->ReverbWidthControl);
	this->SetReverbLevel(this->ReverbLevelControl);
	this->SetChorusState(this->ChorusControlOn);
	this->SetChorusNumber(this->ChorusNumberControl);
	this->SetChorusSpeed(this->ChorusSpeedControl);
	this->SetChorusDepth(this->ChorusDepthControl);
	this->SetChorusLevel(this->ChorusLevelControl);
	this->SetChorusWave(this->ChorusWaveTriangle);
}  // CwxFluidSynth::LoadPresetFile
// -----------------------------------------------------

void CwxFluidSynth::StartMIDI(void)
{
	bool PortFound;
	int PortCount;
	int NumPorts;
	wxString PortName;
	int MaxPrio;
	int SessionResult;

	// System MIDI input
	if (this->Config.UseMIDIInput)
	{
		if (this->Config.MIDIInputName.Length() > 0)
		{
			// For System MIDI, we can not use board index as it may change at any moment if a device is added or removed
			// So we have to find the interface index from its system name
			this->MIDIInput = new RtMidiIn();
			if (MIDIInput)
			{
				PortFound = false;
				NumPorts = MIDIInput->getPortCount();
				if (NumPorts > 0)
				{
					for (PortCount = 0; PortCount < NumPorts; PortCount++)
					{
						PortName = MIDIInput->getPortName(PortCount);
						if (PortName == this->Config.MIDIInputName)
						{
							PortFound = true;
							try
							{
								MIDIInput->openPort(PortCount);
							}
							catch (...)
							{
								wxMessageBox("An error occured while trying to open MIDI IN port " + this->Config.MIDIInputName, "Create synthesizer", wxOK + wxICON_EXCLAMATION);
								delete MIDIInput;
								MIDIInput = 0;
							}
							MIDIInput->ignoreTypes(false, false, false);
							MIDIInput->setCallback(RtMidiCallback, this);
						}
					}
				}
				if (PortFound == false)
				{
					wxMessageBox("MIDI IN port \"" + this->Config.MIDIInputName + " is not available on the computer.", "Create synthesizer", wxOK + wxICON_EXCLAMATION);
				}
			}
		}
	}  // System MIDI input used

	// Activate RTP-MIDI or NetworkUMP handler if they are used
	if (this->Config.NetworkMIDIMode != MIDI_NETWORK_OFF)
	{
		if (this->Config.NetworkMIDIMode == MIDI_NETWORK_RTPMIDI)
		{
			this->RTPMIDIHandler = new CRTP_MIDI(0, RTPMIDIDataCallback, this);
			if (this->RTPMIDIHandler == 0)
			{
				wxMessageBox("Can not create RTP-MIDI communication module", "System error", wxOK + wxICON_ERROR);
			}
			else
			{
				this->RTPMIDIHandler->setSessionName("wxFluid");
				SessionResult = this->RTPMIDIHandler->InitiateSession(this->Config.RemoteDeviceIP, this->Config.RemoteUDPPort, this->Config.RemoteUDPPort+1, this->Config.LocalUDPPort, this->Config.LocalUDPPort + 1, this->Config.IsSessionInitiator);
				switch (SessionResult)
				{
				case 0: break;
				case -1:
					wxMessageBox("Can not open control port for RTP-MIDI session", "RTP-MIDI error", wxOK + wxICON_HAND);
					break;
				case -2:
					wxMessageBox("Can not open data port for RTP-MIDI session", "RTP-MIDI error", wxOK + wxICON_HAND);
					break;
				default:
					wxMessageBox("Can not start RTP-MIDI session", "RTP-MIDI error", wxOK + wxICON_HAND);
				}
			}
		}
		else if (this->Config.NetworkMIDIMode == MIDI_NETWORK_NETUMP)
		{
			this->NetUMPHandler = new CNetUMPHandler(UMPDataCallback, this);
			if (this->NetUMPHandler == 0)
			{
				wxMessageBox("Can not create NetUMP communication module", "System error", wxOK + wxICON_ERROR);
			}
			else
			{
				this->NetUMPHandler->SetEndpointName("wxFluid");
				SessionResult = this->NetUMPHandler->InitiateSession(this->Config.RemoteDeviceIP, this->Config.RemoteUDPPort, this->Config.LocalUDPPort, this->Config.IsSessionInitiator);
				if (SessionResult != 0)
				{
					if (SessionResult == -1)
					{
						wxMessageBox("Can not create UDP port for NetworkUMP session", "NetUMP error", wxOK + wxICON_HAND);
					}
					else
					{
						wxMessageBox("Can not start NetworkUMP session", "NetUMP error", wxOK + wxICON_HAND);
					}
				}
			}
		}

		// Start realtime thread to run the MIDI communication
		MaxPrio = sched_get_priority_max(SCHED_FIFO);
		this->NetMIDIThread = new CThread((ThreadFuncType*)NetMIDIThreadFunc, MaxPrio, this);
		if (this->NetMIDIThread == 0)
		{
			wxMessageBox("Can not create thread for RTP-MIDI/NetUMP communication", "System error", wxOK + wxICON_ERROR);
		}
	}
}  // CwxFluidSynth::StartMIDI
// -----------------------------------------------------

void CwxFluidSynth::StopMIDI(void)
{
	if (NetMIDIThread)
	{
		NetMIDIThread->StopThread(500);
		delete NetMIDIThread;
		NetMIDIThread = 0;
	}

	delete RTPMIDIHandler;
	RTPMIDIHandler = 0;

	delete NetUMPHandler;
	NetUMPHandler = 0;

	// RtMidi MIDI IN port
	if (this->MIDIInput!=0)
	{
		this->MIDIInput->closePort();
		delete this->MIDIInput;
		this->MIDIInput = 0;
	}
}  // CwxFluidSynth::StopMIDI
// -----------------------------------------------------
