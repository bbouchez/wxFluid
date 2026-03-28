/////////////////////////////////////////////////////////////////////////////
// VirtualKeyboard.cpp
// wxFluid virtual keyboard frame
// Copyright:   (c) Benoit BOUCHEZ
/////////////////////////////////////////////////////////////////////////////

#include "VirtualKeyboard.h"

wxBitmap* KeyboardBitmap;

CVirtualKeyboard::CVirtualKeyboard(wxWindow* Parent, CwxFluidSynth* FluidSynth) : CVirtualKeyboardFrame(Parent)
{
	wxString VeloStr;

	this->Synth = FluidSynth;
	this->NoteActive = false;

	for (int VelocityCount = 0; VelocityCount < 128; VelocityCount++)
	{
		VeloStr = wxString::Format("%d", VelocityCount);
		this->OnVelocityChoice->Append(VeloStr);
	}

	this->OnVelocityChoice->SetSelection(64);

#ifdef __TARGET_LINUX__
	this->SetClientSize(870, 270);
#endif // __TARGET_LINUX__
#ifdef __TARGET_MAC__
    this->SetClientSize(810, 230);
#endif // __TARGET_MAC__
}  // CVirtualKeyboard::CVirtualKeyboard
// -----------------------------------------------------

void CVirtualKeyboard::OnClose(wxCloseEvent& WXUNUSED(event))
{
	this->Hide();
}  // CVirtualKeyboard::OnClose
// -----------------------------------------------------

void CVirtualKeyboard::OnLeftMouseDown(wxMouseEvent& event)
{
	int X, Y;
	int Octave = 0;
	int NoteX = 0;
	int NoteOffset;		// 0=C, 1=C#, 2=D, etc...
	int FirstNoteInOctave;
	int NoteNumber;
	int Velocity;

	event.GetPosition(&X, &Y);

	// Define octave
	if (X <= 252)
	{
		Octave = 0;
		NoteX = X;
	}
	else if (X <= 504)
	{
		Octave = 1;
		NoteX = X - 252;
	}
	else if (X <= 756)
	{
		Octave = 2;
		NoteX = X - 504;
	}

	//  Identify note in the octave
	// Special case : if X >= 757, octave is 3 but note is always the same
	if (X > 756)
	{
		Octave = 3;
		NoteOffset = 0;		// C
	}
	else
	{
		// if we are on lower part of keyboard, we only have white keys
		if (Y > 111)
		{
			if (NoteX < 37) NoteOffset = 0;				// C
			else if (NoteX < 73) NoteOffset = 2;		// D
			else if (NoteX < 109) NoteOffset = 4;		// E
			else if (NoteX < 145) NoteOffset = 5;		// F
			else if (NoteX < 181) NoteOffset = 7;		// G
			else if (NoteX < 217) NoteOffset = 9;		// A
			else NoteOffset = 11;						// B
		}
		else
		{
			if (NoteX < 25) NoteOffset = 0;				// C
			else if (NoteX < 49) NoteOffset = 1;		// C#
			else if (NoteX < 61) NoteOffset = 2;		// D
			else if (NoteX < 85) NoteOffset = 3;		// D#
			else if (NoteX < 109) NoteOffset = 4;		// E
			else if (NoteX < 133) NoteOffset = 5;		// F
			else if (NoteX < 157) NoteOffset = 6;		// F#
			else if (NoteX < 169) NoteOffset = 7;		// G
			else if (NoteX < 192) NoteOffset = 8;		// G#
			else if (NoteX < 205) NoteOffset = 9;		// A
			else if (NoteX < 229) NoteOffset = 10;		// A#
			else NoteOffset = 11;
		}
	}

	// Compute MIDI Note number depending on octave, note offset and octave spin selector
	FirstNoteInOctave = (this->SpinOctave->GetValue() + 2) * 12;
	NoteNumber = FirstNoteInOctave + NoteOffset + (12 * Octave);
	if (NoteNumber > 127) NoteNumber = 127;

	this->NotePlayed = NoteNumber;
	this->PlayedUMPChannel = this->ChannelChoice->GetSelection();
	this->NoteActive = true;

	Velocity = this->OnVelocityChoice->GetSelection();

	if (Velocity == 0) Velocity = 1;		// UMP specification says that velocity 0 shall be replaced by 1 for MIDI 1.0

	fluid_synth_noteon(Synth->synth, this->PlayedUMPChannel, this->NotePlayed, Velocity);
}  // CVirtualKeyboard::OnLeftMouseDown
// -----------------------------------------------------

void CVirtualKeyboard::OnLeftMouseUp(wxMouseEvent& WXUNUSED(event))
{
	this->GenerateNoteOff();
}  // CVirtualKeyboard::OnLeftMouseUp
// -----------------------------------------------------

void CVirtualKeyboard::OnMouseLeave(wxMouseEvent& WXUNUSED(event))
{
	this->GenerateNoteOff();
}  // CVirtualKeyboard::OnMouseLeave
// -----------------------------------------------------

void CVirtualKeyboard::DrawKeyboard(void)
{
#ifdef __TARGET_MAC__
    // On Mac, the panel captures the mouse events so we need to draw the bitmap directly on parent
    // Bitmap must be shifted "manually" as the center alignment of panel is not taken into account
    wxClientDC dc(this);
    dc.DrawBitmap(*KeyboardBitmap, 10, 0, false);
#else
	wxClientDC dc(this->KeyPanel);
    dc.DrawBitmap(*KeyboardBitmap, 0, 0, false);
#endif
}  // CVirtualKeyboard::DrawKeyboard
// -----------------------------------------------------

void CVirtualKeyboard::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	this->DrawKeyboard();
}  // CVirtualKeyboard::OnPaint
// -----------------------------------------------------

void CVirtualKeyboard::GenerateNoteOff(void)
{
	if (this->NoteActive == false) return;

	fluid_synth_noteoff(Synth->synth, this->PlayedUMPChannel, this->NotePlayed);

	this->NoteActive = false;
}  // CVirtualKeyboard::GenerateNoteOff
// -----------------------------------------------------
