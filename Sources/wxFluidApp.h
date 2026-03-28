/////////////////////////////////////////////////////////////////////////////
// Name:        wxFluidApp.h
// Purpose:     wxFluid Main application module
// Author:      Benoit BOUCHEZ
// Created:     
// Copyright:   (c) Benoit BOUCHEZ
/////////////////////////////////////////////////////////////////////////////

#ifndef __WXFLUIDAPP_H__
#define __WXFLUIDAPP_H__

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

class wxFluidApp : public wxApp
{
public:
    virtual bool OnInit();		
	virtual int OnExit();
};

#endif