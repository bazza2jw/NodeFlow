/*
 * Copyright (C) 2022 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */

#ifndef NODEEDITORCHILD_H
#define NODEEDITORCHILD_H
#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#   include "wx/wx.h"
#endif
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/toolbar.h>
#include <wx/stattext.h>

#include "NodeEditor.h"
#include <wx/mdi.h>
#include "canvaswindows.h"

#include "NodeEditorPanel.h"

class NodeEditorChild : public 	wxMDIChildFrame
{
    NodeEditorPanel * _panel = nullptr;
protected:
    wxBoxSizer* canvasSizer = nullptr;
    CanvasWindows * _canvas = nullptr;

public:
    NodeEditorChild(wxMDIParentFrame * parent);
    virtual ~NodeEditorChild();
    void onClose(wxCommandEvent& event);

};
#endif // NODEEDITORCHILD_H
