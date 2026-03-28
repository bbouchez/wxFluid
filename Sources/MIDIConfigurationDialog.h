/////////////////////////////////////////////////////////////////////////////
// Name:        MIDIConfigurationDialog.h
// Purpose:     wxFluid MIDI configuration dialog
// Created:     28/04/2024
// Copyright:   (c) Benoit BOUCHEZ
/////////////////////////////////////////////////////////////////////////////

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
