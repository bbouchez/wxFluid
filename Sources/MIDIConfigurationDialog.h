/*
 *  MIDIConfigurationDialog.h
 *  wxFluid Synthesizer
 *  wxFluid MIDI / RTP-MIDI / Network MIDI 2.0 configuration dialog
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

#ifndef __MIDICONFIGDIALOG_H__
#define __MIDICONFIGDIALOG_H__

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "PropertiesForm.h"
#include "SynthClass.h"

class CFluidMIDIConfigDialog : public CMIDIConfigDialog
{
public:
	CFluidMIDIConfigDialog(wxWindow* Parent, TWXFLUIDCONFIG* Config);

	void OnOKBtn(wxCommandEvent& event);
	void OnCancelBtn(wxCommandEvent& event);
private:
	TWXFLUIDCONFIG* PConfig;
};

#endif
