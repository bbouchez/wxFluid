/////////////////////////////////////////////////////////////////////////////
// Name:        KnobControllers.h
// Purpose:     wxFluid MIDI controllers frame
// Copyright:   (c) Benoit BOUCHEZ
/////////////////////////////////////////////////////////////////////////////

#ifndef __KNOBCONTROLLERS_H__
#define __KNOBCONTROLLERS_H__

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wxFluidControllersForm.h"
#include "SynthClass.h"
#include "wxAnimKnob.h"

class CMIDIControllersFrame : public CKnobFrame
{
public:
	CMIDIControllersFrame(wxWindow* Parent);
	~CMIDIControllersFrame();

	void OnClose(wxCloseEvent& event);
private:
	wxDECLARE_EVENT_TABLE();

	wxBitmap* KnobBitmap;
	wxAnimKnob* Knob[8];

	void OnKnob(wxCommandEvent& event);
};

#endif
