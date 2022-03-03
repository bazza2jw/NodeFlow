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

#include "NodeFlowEditorDialog.h"
#include "NodeEditorPanel.h"
/*!
 * \brief NodeFlowEditorDialog::NodeFlowEditorDialog
 * \param parent
 */
NodeFlowEditorDialog::NodeFlowEditorDialog(wxWindow* parent, const std::string &s)
    : NodeFlowEditorDialogBase(parent),_fileName(s)
{
    _editor = new NodeEditorPanel(this);
    PanelSizer->Add(_editor, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
}
/*!
 * \brief NodeFlowEditorDialog::~NodeFlowEditorDialog
 */
NodeFlowEditorDialog::~NodeFlowEditorDialog()
{
}

void NodeFlowEditorDialog::onOk(wxCommandEvent& /*event*/)
{
    EndModal(wxID_OK);
}
void NodeFlowEditorDialog::onOpen(wxCommandEvent& event)
{
    _editor->onOpen(event);
}
void NodeFlowEditorDialog::onSave(wxCommandEvent& event)
{
    _editor->onSave(event);
}
void NodeFlowEditorDialog::onSaveAs(wxCommandEvent& event)
{
    _editor->onSave(event);
}
