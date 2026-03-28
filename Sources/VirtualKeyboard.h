/////////////////////////////////////////////////////////////////////////////
// Name:        VirtualKeyboard.h
// Purpose:     wxFluid virtual keyboard frame
// Author:      Benoit BOUCHEZ
// Created:     
// Copyright:   (c) Benoit BOUCHEZ
/////////////////////////////////////////////////////////////////////////////
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
