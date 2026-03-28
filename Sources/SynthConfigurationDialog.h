/////////////////////////////////////////////////////////////////////////////
// Name:        ConfigurationDialog.h
// Purpose:     wxFluid configuration dialog
// Author:      Benoit BOUCHEZ
// Created:     
// Copyright:   (c) Benoit BOUCHEZ
/////////////////////////////////////////////////////////////////////////////

#ifndef __CONFIGDIALOG_H__
#define __CONFIGDIALOG_H__

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "PropertiesForm.h"
#include "SynthClass.h"

class CFluidConfigDialog : public CSynthConfigDialog
{
public:
	CFluidConfigDialog(wxWindow* Parent, TWXFLUIDCONFIG* Config);

	void OnOKBtn(wxCommandEvent& event);
	void OnCancelBtn(wxCommandEvent& event);
private:
	TWXFLUIDCONFIG* PConfig;

	void OnAudioDriverChoice(wxCommandEvent& event);

#ifdef __TARGET_WIN__
	void FillMMEDeviceList(wxString& SelectedDeviceName);
	//! Fill the choice box with DirectSound devices and returns the number of output devices
	int FillDirectSoundDeviceList(wxString& SelectedDeviceName);
	//! Fill the choice box with WASAPI devices and returns the number of output devices
	int FillWASAPIDeviceList(wxString& SelectedDeviceName);
#endif
#ifdef __TARGET_LINUX__
    int ListALSADevices(wxString& SelectedDeviceName);
    void SelectJACK(void);
#endif
#ifdef __TARGET_MAC__
    int FillCoreAudioDeviceList(wxString& SelectedDeviceName);
#endif
};

#endif
