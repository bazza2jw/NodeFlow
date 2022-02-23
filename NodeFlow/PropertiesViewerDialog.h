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

#ifndef PROPERTIESVIEWERDIALOG_H
#define PROPERTIESVIEWERDIALOG_H
#include "PropertyPanels.h"
#include <MrlLib/mrllib.h>
#include <MrlLib/variantpropertytree.h>
/*!
 * \brief The PropertiesViewerDialog class
 */
class  PropertiesViewerDialog : public PropertiesViewerDialogBase
{
    MRL::VariantPropertyTree &_data;
    MRL::PropertyPath _path;
public:
    PropertiesViewerDialog(wxWindow *parent, MRL::VariantPropertyTree &data, MRL::PropertyPath &p) :
        PropertiesViewerDialogBase(parent),_data(data),_path(p)
    {

    }

    virtual ~PropertiesViewerDialog() {}
protected:
    virtual void OnClose(wxCommandEvent& event);
};
#endif // PROPERTIESVIEWERDIALOG_H
