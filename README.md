# wxFluid
FluidSynth based SoundFont synthesizer with RTP-MIDI and Network MIDI 2.0 support. It has been written to run under Raspberry Pi OS, Windows and macOS.

<img width="967" height="290" alt="MainWindow" src="https://github.com/user-attachments/assets/a62d5fe3-d378-49a3-9ca7-4b249acfccb5" />

<img width="1268" height="670" alt="macOS" src="https://github.com/user-attachments/assets/d43432bc-2a53-4078-a7ad-e68282f626aa" />

![1774858849629](https://github.com/user-attachments/assets/81a31a14-5215-41ba-b6fc-09d8997d7346)


## Installing wxFluid
### Windows
Download the installer executable from the Releases page and launch it.
The installer normally installs automatically the MS Visual C runtime libraries required by wxFuild. On some Windows machines, it can happen that one of the runtime (for pthreads) fails during installation. In that case, simply go into the installation directory of wxFluid. You will find the runtime installer, which can be launcher manually from here (you need to launch them as administrator)

### MacOS
Download the zip from the Releases page and simply unzip the content on the Mac disk drive. The zip file contains the complete application which should appear as it in MacOS once the zip file content is copied.
You will need also to install libfluidsynth. I recommend to use the MacPorts to install it.
Once MacPorts is installed, open a terminal and enter the following command :

sudo port install fluidsynth

The first time wxFluid is launched under MacOS, you will probably get a warning from MacOS telling that application was downloaded from Internet and is not safe. Just follow indications from here to allow ump-ox : https://support.apple.com/en-us/102445

Once the app is authorized a first time, it becomes automatically valid for your system and the safety warning will not be displayed again.

### Raspberry Pi OS
wxFluid requires two libraries to be installed on Raspberry Pi OS :
- libasound
- libfluidsynth

It is highly recommended to use JACK as audio server for wxFluid. Consequently, you should also install JACk and QJACKCTL on the Raspberry.

Once all libraries are installed, simply unzip the RPi executable file from the Release page and give it executable rights, by entering the following command in the executable directory :

sudo chmod +x wxFluid

## Synthesizer configuration
Before wxFluid can be used, the audio engine must be configured, in particular for the audio driver. This is done by Options / Synthesizer menu.

- On Windows, it is recommended to use the WASAPI driver.
- On MacOs, you must use CoreAudio
- On Raspberry Pi, it is recommended to use JACK

You must also configure the buffer size and number of buffers. The values must be adjusted according to the system performances in order to avoid clicks on the audio outputs.

<img width="596" height="336" alt="AudioDriver" src="https://github.com/user-attachments/assets/b61690b2-5ff4-4aa9-8587-5e301465bcad" />

## MIDI configuration
wxFluid can be controlled by MIDI from three different sources :
- the system MIDI driver (from MME on Windows, CoreMIDI on Mac and ALSA on Raspberry)
- RTP-MIDI as a session initiator or session listener
- Network MIDI 2.0 as a session initiator or session listener

Note that RTP-MIDI and Network MIDI 2.0 do not require **any** system driver to be installed, wxFluid uses its own communication daemon to communicate using these two standards.

MIDI Configuration is done via Options / MIDI Configuration menu

<img width="596" height="421" alt="MIDIConfiguration" src="https://github.com/user-attachments/assets/c291c985-bc9e-4fa9-8b3c-2e571d730717" />

If you want to use the system MIDI driver to control wxFluid, check the "Use MIDI Input" checkbox and select the MIDI interface you want to use.

If you want to use RTP-MIDI or Network MIDI 2.0, select the desired mode in the Network MIDI panel and enter the IP communication parameters to use with your MIDI network interface. If the "Active" checkbox is checked, wxFluid becomes a session initiator and will invite the remote device. If this checkbox is unchecked, wxFluid becomes a session listener and will need an external session initiator to open the session.

## SoundFont / Preset assignation
wxFluid can use up to 16 SoundFont files at the same time, and each MIDI channel can be associated to a different MIDI preset (each channel can then use a preset from the same SoundFont or from a different SoundFont)

You can create as many configuration as needed, each configuration having its own set of SoundFonts and presets.

To load a SoundFont, click on "Configure SoundFont / Preset" button the main window.

<img width="850" height="473" alt="SoundFontSlot" src="https://github.com/user-attachments/assets/b4081ed3-facc-4f8c-959a-ae6fbf4c27ca" />

You can either double-click on a slot or left-click on a slot then click on "Load SoundFont in slot" to choose the SoundFont you want to use for a given slot.

Once the SoundFonts are assigned to the various slots, click on the "Presets" tab to choose which preset you want to assign to each MIDI channel.

<img width="850" height="473" alt="Presets" src="https://github.com/user-attachments/assets/beab9a31-f8c1-4ba3-b9c5-ef265f0ec098" />

- Select first the SoundFont you want to use for the MIDI channel by clicking on the cell to display the list of available SoundFonts. Multiple MIDI channels can use the same SoundFont if the SoundFont contains multiple presets
- Enter the bank MSB/LSB (you can also enter a bank number in the form of a value from 0 to 16384)
- Select the preset you want to use in the selected SoundFont and bank by clicking in the cell in the Preset column

Once the preset configuration is created, you can save it on the disk using the File menu.

## Compiling wxFluid
wxFluid has been written to be compilable and ran on the following platforms :
* Windows 64-bit 
* macOS
* Raspberry OS

wxFluid can normally be compiled and run on other Linux platforms (especially Debian and Ubuntu), but I have not (yet) made the test.

The following libraries are needed in order to compile wxFluid :
* RTP-MIDI (https://github.com/bbouchez/RTP-MIDI)
* NetworkUMP (https://github.com/bbouchez/NetUMP)
* BEB SDK (https://github.com/bbouchez/BEBSDK)
* wxIndustrialControls (https://github.com/bbouchez/wxIndustrialControls)
* rtmidi version 5.0 (https://github.com/thestk/rtmidi/releases)
* rtaudio 6.0 (https://github.com/thestk/rtaudio/releases)
* wxWidgets 3.2 (https://github.com/wxWidgets/wxWidgets)

wxWidgets library must be compiled as a static 64-bit library before it can be included in wxFluid. Refer to wxWidgets wiki for detaild instructions to perform this step.

The following defines are required for the various libraries to be built correctly :
#define __TARGET_WIN__ to compile for the Windows platform
#define __TARGET_MAC__ to compile for the macOS platform
#define __TARGET_LINUX__ to compile for the Raspberry Pi platform





