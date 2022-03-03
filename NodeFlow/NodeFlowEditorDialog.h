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

#ifndef NODEFLOWEDITORDIALOG_H
#define NODEFLOWEDITORDIALOG_H
#include "NodeEditor.h"
class NodeEditorPanel;
class NodeFlowEditorDialog : public NodeFlowEditorDialogBase
{
    const std::string _fileName;
    NodeEditorPanel * _editor = nullptr;
public:
    NodeFlowEditorDialog(wxWindow* parent, const std::string &s);
    virtual ~NodeFlowEditorDialog();
    NodeEditorPanel * editor() const { return _editor;}
public:
    virtual void onOk(wxCommandEvent& event);
    virtual void onOpen(wxCommandEvent& event);
    virtual void onSave(wxCommandEvent& event);
    virtual void onSaveAs(wxCommandEvent& event);
};
#endif // NODEFLOWEDITORDIALOG_H
