/*
 *  KnobControllers.cpp
 *  wxFluid Synthesizer
 *  wxFluid MIDI controllers frame
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

#include "KnobControllers.h"

#define ID_KNOB			1000

wxBEGIN_EVENT_TABLE(CMIDIControllersFrame, wxFrame)
EVT_KNOB_CHANGE(ID_KNOB, CMIDIControllersFrame::OnKnob)
wxEND_EVENT_TABLE()

CMIDIControllersFrame::CMIDIControllersFrame(wxWindow* Parent) : CKnobFrame(Parent)
{
#ifdef __TARGET_MAC__
    this->KnobBitmap = new wxBitmap ("knob", wxBITMAP_TYPE_PNG_RESOURCE);
#else
	this->KnobBitmap = new wxBitmap(wxT("images/knob.png"), wxBITMAP_TYPE_PNG);
#endif

	this->Knob[0] = new wxAnimKnob(this->KnobPanel1, ID_KNOB, this->KnobBitmap, 66, 128, wxPoint(10, 10), wxSize(66, 66), wxBORDER_NONE);
	this->Knob[1] = new wxAnimKnob(this->KnobPanel2, ID_KNOB, this->KnobBitmap, 66, 128, wxPoint(10, 10), wxSize(66, 66), wxBORDER_NONE);
	this->Knob[2] = new wxAnimKnob(this->KnobPanel3, ID_KNOB, this->KnobBitmap, 66, 128, wxPoint(10, 10), wxSize(66, 66), wxBORDER_NONE);
	this->Knob[3] = new wxAnimKnob(this->KnobPanel4, ID_KNOB, this->KnobBitmap, 66, 128, wxPoint(10, 10), wxSize(66, 66), wxBORDER_NONE);

	this->Knob[4] = new wxAnimKnob(this->KnobPanel5, ID_KNOB, this->KnobBitmap, 66, 128, wxPoint(10, 10), wxSize(66, 66), wxBORDER_NONE);
	this->Knob[5] = new wxAnimKnob(this->KnobPanel6, ID_KNOB, this->KnobBitmap, 66, 128, wxPoint(10, 10), wxSize(66, 66), wxBORDER_NONE);
	this->Knob[6] = new wxAnimKnob(this->KnobPanel7, ID_KNOB, this->KnobBitmap, 66, 128, wxPoint(10, 10), wxSize(66, 66), wxBORDER_NONE);
	this->Knob[7] = new wxAnimKnob(this->KnobPanel8, ID_KNOB, this->KnobBitmap, 66, 128, wxPoint(10, 10), wxSize(66, 66), wxBORDER_NONE);
}  // CMIDIControllersFrame::CMIDIControllersFrame
// -----------------------------------------------------

CMIDIControllersFrame::~CMIDIControllersFrame()
{
	for (int i = 0; i < 8; i++)
	{
		delete this->Knob[i];
		this->Knob[i] = 0;
	}

	delete this->KnobBitmap;
	this->KnobBitmap = 0;
}  // CMIDIControllersFrame::~CMIDIControllersFrame
// -----------------------------------------------------

void CMIDIControllersFrame::OnClose(wxCloseEvent& WXUNUSED(event))
{
	this->Hide();
}  // CMIDIControllersFrame::OnClose
// -----------------------------------------------------

void CMIDIControllersFrame::OnKnob(wxCommandEvent& event)
{
	wxObject* Sender;

	// Identify sending knob
	Sender = event.GetEventObject();
}  // CMIDIControllersFrame::OnKnob
// -----------------------------------------------------
