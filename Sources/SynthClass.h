/////////////////////////////////////////////////////////////////////////////
// SynthClass.h
// FluidSynth Synthesizer engine class
// (c) 2024 - Benoit BOUCHEZ
/////////////////////////////////////////////////////////////////////////////

#ifndef __SYNTHCLASS_H__
#define __SYNTHCLASS_H__

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#ifdef __TARGET_MAC__
#include "/opt/local/include/fluidsynth.h"
#else
#include "fluidsynth.h"
#endif
#include "wxFluidAppConfig.h"
#include "RtMidi.h"
#include "RTP_MIDI.h"
#include "NetUMP.h"
#include "CThread.h"
#include "SystemSleep.h"

#define WINDOWS_MME_DRIVER			0	
#define WINDOWS_DIRECTSOUND_DRIVER	1
#define WINDOWS_WASAPI_DRIVER		2

#define LINUX_JACK_DRIVER			0
#define LINUX_ALSA_DRIVER			1

#define MIDI_NETWORK_OFF			0
#define MIDI_NETWORK_RTPMIDI		1
#define MIDI_NETWORK_NETUMP			2

// Default values for reverb and chorus
#define DEFAULT_VOLUME              0.33f
#define DEFAULT_REVERB_ROOM_SIZE    0.5f
#define DEFAULT_REVERB_DAMPING      0.0f
#define DEFAULT_REVERB_WIDTH        0.6f
#define DEFAULT_REVERB_LEVEL        0.0f
#define DEFAULT_CHORUS_NUMBER       0.5f
#define DEFAULT_CHORUS_SPEED        0.15f
#define DEFAULT_CHORUS_DEPTH        0.5f
#define DEFAULT_CHORUS_LEVEL        0.0f

//! Maximum number of SF2 files that can be loaded in the engine
#define MAX_SOUND_FONTS				16

typedef struct {
	// Audio parameters
	unsigned int AudioDriverType;		// 0 : MME, 1 : DirectSound, 2 : WASAPI on Windows
	wxString AudioDeviceName;
	unsigned int AudioPeriods;
	unsigned int AudioBufferSize;
	bool ExclusiveWasapi;
	unsigned int SampleRate;
	// TODO : add sample format (16 bits, float)

	// MIDI parameters
	bool UseMIDIInput;					// Only for system MIDI
	wxString MIDIInputName;
	unsigned int MIDIChannels;
	unsigned int ProgChangeMode;		// 0 : GM, 1 : GS, 2 : MMA, 3 : XG
	unsigned int NetworkMIDIMode;		// 0 : no network MIDI, 1 : RTP-MIDI, 2 : NetUMP
	bool IsSessionInitiator;
	unsigned int RemoteDeviceIP;
	unsigned short LocalUDPPort;		// UDP port RTP-MIDI and Network UMP
	unsigned short RemoteUDPPort;		// UDP port of remote device when wxFluid is session initiator

	// Synth parameters
	unsigned int SynthStereoPairs;
	unsigned int SynthAudioGroups;
	unsigned int SynthMaxPolyphony;
} TWXFLUIDCONFIG;

class CwxFluidSynth {
public:
	CwxFluidSynth(void);
	~CwxFluidSynth();

	int CreateFluidEngine(void);
	void StopSynth(void);

	//! System MIDI callback
	void RTMIDICallback(std::vector< unsigned char > *message);
    //! RTP-MIDI callback
    void ProcessRTPMIDI(unsigned int DataSize, unsigned char* DataBlock);
    //! NetUMP callback
    void ProcessNetUMP(uint32_t* DataBlock);

	TWXFLUIDCONFIG Config;
	fluid_synth_t* synth;

	//! SoundFont identification
	// WARNING : SoundFont slot number IS NOT the SFID
	// Each slot contains SoundFont path and SFID if the SoundFont has been loaded correctly
	int FontID[MAX_SOUND_FONTS];							// -1 if SoundFont has not been loaded correctly (initialized when SF is loaded)
	wxString SoundFontFilePath[MAX_SOUND_FONTS];			// SF path is saved in the preset file

	// Preset management : one preset per MIDI channel
	int SoundFontSlotForMIDIChannel[16];					
	unsigned int PresetBank[16];							// MIDI Bank MSB + LSB : 0..16383
	unsigned int ProgNumber[16];							// MIDI Program Number : 0..127
    
    // Images of controls on panel
    float VolumeControl;
    bool ReverbControlOn;
    float ReverbRoomSizeControl;
    float ReverbDampingControl;
    float ReverbWidthControl;
    float ReverbLevelControl;
    bool ChorusControlOn;
    int ChorusNumberControl;
    float ChorusSpeedControl;
    float ChorusDepthControl;
    float ChorusLevelControl;
	bool ChorusWaveTriangle;

	bool PresetConfigurationChanged;

	//! Load a SoundFont file in the engine
	//\! returns SoundFont ID
	int LoadSoundFont(wxString& SFPath, unsigned int Position);
	//! Unload the soundfont loaded in the indicated slot
	void UnloadSoundFont(unsigned int Position);

	// Methods to adjust font panel parameters
	void SetVolume(float Volume);
	void SetReverbState(bool ControlValue);
	void SetReverbRoomSize(float Size);
	void SetReverbDamping(float Damping);
	void SetReverbWidth(float Width);
	void SetReverbLevel(float Level);

	void SetChorusState(bool ControlValue);
	void SetChorusNumber(unsigned int Number);
	void SetChorusSpeed(float Speed);
	void SetChorusDepth(float Depth);
	void SetChorusLevel(float Level);
	void SetChorusWave(bool IsTriangle);

	int SaveEngineConfiguration(void);
	int LoadEngineConfiguration(void);

	int SavePresetConfiguration(wxString PresetFileName);

	//! Stop all notes and unload all soundfonts
	void ClearPresets(void);
	//! Load a presets file into synth (loads soundfonts and initialize all channels)
	void LoadPresetFile(wxString PresetFileName);

	//! Start MIDI processing based on configuration parameters
	void StartMIDI(void);
	//! Stop the MIDI communication handlers
	void StopMIDI(void);

	//! Method to call every millisecond to run RTP-MIDI and Network UMP communication in background
	void RunNetworkMIDI(void);
    
    //! These two members must be public in order to get the connection status for the GUI
    CRTP_MIDI* RTPMIDIHandler;
    CNetUMPHandler* NetUMPHandler;
private:
	fluid_settings_t* settings;
	fluid_audio_driver_t* adriver;
	RtMidiIn* MIDIInput;
	CThread* NetMIDIThread;
    
    //! Send MIDI1.0 message from communication modules to synth engine
    void SendMIDI1ToFluid (uint8_t* MIDIMsg);

	//! Load preset data into memory but does not load soundfonts and does not initialize channels
	int LoadPresetConfiguration(wxString& PresetFileName);
};

#endif
