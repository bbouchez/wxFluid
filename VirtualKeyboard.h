/*
 *  VirtualKeyboard.h
 *  wxFluid Synthesizer
 *  wxFluid virtual keyboard frame
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

#ifndef __VIRTUALKEYBOARD_H__
#define __VIRTUALKEYBOARD_H__

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "VirtualKeyboardForms.h"
#include "SynthClass.h"

class CVirtualKeyboard : public CVirtualKeyboardFrame
{
public:
	CVirtualKeyboard(wxWindow* Parent, CwxFluidSynth* FluidSynth);

	void OnClose(wxCloseEvent &event);
	void OnPaint(wxPaintEvent& event);

	void OnLeftMouseDown(wxMouseEvent& event);
	void OnLeftMouseUp(wxMouseEvent& event);
	void OnMouseLeave(wxMouseEvent& event);
private:
	int NotePlayed;		// MIDI note number currently played
	bool NoteActive;	// A Note On message has been generated
	int PlayedUMPChannel;
	CwxFluidSynth* Synth;

	void DrawKeyboard(void); 
	void GenerateNoteOff(void);
};

#endif
