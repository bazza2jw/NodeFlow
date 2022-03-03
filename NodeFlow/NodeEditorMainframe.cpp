/*
 * Copyright (C) 2022 -  B. J. Hill
 *
 * This file is part of NodeFlow. NodeFlow C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */

#include "NodeEditorMainframe.h"
#include <wx/app.h>
#include "NodeEditorChild.h"
/*!
 * \brief NodeEditorMainframe::NodeEditorMainframe
 * \param parent
 */
NodeEditorMainframe::NodeEditorMainframe(wxWindow* parent)
    : NodeEditorMainframeBase(parent)
{

}

NodeEditorMainframe::~NodeEditorMainframe()
{
}

void NodeEditorMainframe::onExit(wxCommandEvent& event)
{
    wxApp::GetInstance()->ExitMainLoop();
}
void NodeEditorMainframe::onNew(wxCommandEvent& event)
{
    // add a new page
    NodeEditorChild *w = new NodeEditorChild(this);
    w->Show();
}
