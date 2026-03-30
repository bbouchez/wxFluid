/*
 *  wxFluidApp.cpp
 *  wxFluid Synthesizer
 *  wxFluid main application module
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

/* Release notes :
0.1.0 - rc1 : 18/07/2022
	- Proof of concept version

1.1.0 : 02/01/2025
	- First release for NAMM demo

1.2.0 : 03/01/2025
	- Resized windows depending on platforms
	- Added display of Network UMP and RTP-MIDI connection status
	- Removed manual release of objects in the OnClose() method of main window (generates a segfault on Linux)
	- Bitmaps for controls set as globals, freed after main window is destroyed
	- Bitmaps set as resources for Windows platform so the /image folder is not required anymore
	- Update to libfluidsynth V2.4.2 for Windows

1.3.0 : 05/01/2025
    - Added warning message when new preset is loaded or preset is cleared while config is not saved
	- Added warning message when application tries to close while soundfont config is not saved

2.0.0 : 25/03/2026
	- Switch pthread_win32 in place of phtread_vc2
	- Removed call to pthread_exit() which generates a memory leak on Windows
	- Changed order of columns in the Preset selector to help user understand he needs to select the SoundFont first...
	- Memory leak corrected (SoundFond selector dialog was not deleted after closing)
*/

#include "wxFluidApp.h"
#include "wxFluidMainFrame.h"
#include "network.h"

extern wxBitmap* KeyboardBitmap;

bool wxFluidApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

    wxInitAllImageHandlers();

    // Load all global bitmaps
#ifdef __TARGET_MAC__
    KnobBitmap = new wxBitmap("knob", wxBITMAP_TYPE_PNG_RESOURCE);
	ToggleBitmapOff = new wxBitmap("ButtonLED_OFF", wxBITMAP_TYPE_PNG_RESOURCE);
	ToggleBitmapOn = new wxBitmap("ButtonLED_ON", wxBITMAP_TYPE_PNG_RESOURCE);
	WaveBitmapSine = new wxBitmap("ChorusWaveSine", wxBITMAP_TYPE_PNG_RESOURCE);
	WaveBitmapTri = new wxBitmap("ChorusWaveTri", wxBITMAP_TYPE_PNG_RESOURCE);
	KeyboardBitmap = new wxBitmap("keyboard", wxBITMAP_TYPE_PNG_RESOURCE);
	IconBitmap = new wxBitmap("icon256", wxBITMAP_TYPE_PNG_RESOURCE);
#endif
#ifdef __TARGET_WIN__
	KnobBitmap = new wxBitmap("knob", wxBITMAP_TYPE_PNG_RESOURCE);
	ToggleBitmapOff = new wxBitmap("buttonled_off", wxBITMAP_TYPE_PNG_RESOURCE);
	ToggleBitmapOn = new wxBitmap("buttonled_on", wxBITMAP_TYPE_PNG_RESOURCE);
	WaveBitmapSine = new wxBitmap("choruswavesine", wxBITMAP_TYPE_PNG_RESOURCE);
	WaveBitmapTri = new wxBitmap("choruswavetri", wxBITMAP_TYPE_PNG_RESOURCE);
	KeyboardBitmap = new wxBitmap("keyboard", wxBITMAP_TYPE_PNG_RESOURCE);
	IconBitmap = new wxBitmap("icon256", wxBITMAP_TYPE_PNG_RESOURCE);
#endif
#ifdef __TARGET_LINUX__
	KnobBitmap = new wxBitmap(wxT("images/knob.png"), wxBITMAP_TYPE_PNG);
	ToggleBitmapOff = new wxBitmap("images/ButtonLED_OFF.png", wxBITMAP_TYPE_PNG);
	ToggleBitmapOn = new wxBitmap("images/ButtonLED_ON.png", wxBITMAP_TYPE_PNG);
	WaveBitmapSine = new wxBitmap("images/ChorusWaveSine.png", wxBITMAP_TYPE_PNG);
	WaveBitmapTri = new wxBitmap("images/ChorusWaveTri.png", wxBITMAP_TYPE_PNG);
	KeyboardBitmap = new wxBitmap("images/keyboard.png", wxBITMAP_TYPE_PNG_RESOURCE);
	IconBitmap = new wxBitmap("images/icon256.png", wxBITMAP_TYPE_PNG_RESOURCE);
#endif

#ifdef __TARGET_WIN__
	if (OpenNetwork() == false)
		wxMessageBox("Can not start Winsock layer", "Critical error", wxOK + wxICON_ERROR);
	timeBeginPeriod(1);
#endif

	wxFluidMainFrame* frame = new wxFluidMainFrame(0);
    frame->Show(true);

    return true;
}  // wxFluidApp::OnInit()
// -----------------------------------------------------

int wxFluidApp::OnExit()
{
#ifdef __TARGET_WIN__
	timeEndPeriod(1);
	CloseNetwork();
#endif

	wxApp::OnExit();  // Call ancestor

	// Delete all bitmaps used by controls now that main window is destroyed
    delete KnobBitmap;
	KnobBitmap = 0;

	delete ToggleBitmapOff;
	ToggleBitmapOff = 0;

	delete ToggleBitmapOn;
	ToggleBitmapOn = 0;

	delete WaveBitmapSine;
	WaveBitmapSine = 0;

	delete WaveBitmapTri;
	WaveBitmapTri = 0;

	delete KeyboardBitmap;
	KeyboardBitmap = 0;

	delete IconBitmap;
	IconBitmap = 0;

	return 0;
}  // wxFluidApp::OnExit
// -----------------------------------------------------
