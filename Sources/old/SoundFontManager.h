/////////////////////////////////////////////////////////////////////////////
// SoundFontManager.h
// wxFluid SoundFont management (load / unload)
// (c) Benoit BOUCHEZ
/////////////////////////////////////////////////////////////////////////////

#ifndef __SOUNDFONTMANAGER_H__
#define __SOUNDFONTMANAGER_H__

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wxFluidFontSelectorForm.h"
#include "SynthClass.h"

class CSoundFontManager : public CSoundFontManagerDialog
{
public:
	CSoundFontManager(wxWindow* Parent, CwxFluidSynth* FluidSynth);

	void OnBtnLoad(wxCommandEvent& event);
	void OnBtnUnload(wxCommandEvent& event);

	void OnGridSelected(wxGridEvent& event);
private:
	CwxFluidSynth* Synth;
	int SelectedLine;
};


#endif