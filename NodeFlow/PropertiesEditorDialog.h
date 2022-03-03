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

#ifndef PROPERTIESEDITORDIALOG_H
#define PROPERTIESEDITORDIALOG_H
#include "PropertyPanels.h"
#include <MrlLib/mrllib.h>
#include <MrlLib/variantpropertytree.h>
#include "propertiesloader.h"
/*!
 * \brief The PropertiesEditorDialog class
 */
class  PropertiesEditorDialog : public PropertiesEditorDialogBase
{
    PropertiesLoader _loader;
    MRL::VariantPropertyTree &_data;
    MRL::PropertyPath &_path;
public:
    PropertiesEditorDialog(wxWindow* parent, MRL::VariantPropertyTree &data,  MRL::PropertyPath &p);
    virtual ~PropertiesEditorDialog();
    PropertiesLoader & loader() { return _loader;}
 protected:
    //
    virtual void OnNotes(wxCommandEvent& event);
    virtual void OnSettings(wxCommandEvent& event);
    virtual void OnOk(wxCommandEvent& event);
    //
    virtual void load() {} // read from tree
    virtual void save() {} // write to tree
    //
    MRL::VariantPropertyTree & data() { return _data;}
    MRL::PropertyPath & path() { return _path;}
};
#endif // PROPERTIESEDITORDIALOG_H
