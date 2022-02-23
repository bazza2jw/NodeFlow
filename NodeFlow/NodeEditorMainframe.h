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

#ifndef NODEEDITORMAINFRAME_H
#define NODEEDITORMAINFRAME_H
#include "NodeEditor.h"
#include "canvaswindows.h"

class NodeEditorMainframe : public NodeEditorMainframeBase
{
    CanvasWindows * _canvas = nullptr;
public:
    NodeEditorMainframe(wxWindow* parent);
    virtual ~NodeEditorMainframe();
public:
    virtual void onNew(wxCommandEvent& event);
    virtual void onExit(wxCommandEvent& event);
};
#endif // NODEEDITORMAINFRAME_H
