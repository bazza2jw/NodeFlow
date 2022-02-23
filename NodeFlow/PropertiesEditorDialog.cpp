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

#include "PropertiesEditorDialog.h"
#include "propertiesloader.h"

/*!
 * \brief PropertiesEditorDialog::PropertiesEditorDialog
 * \param parent
 * \param p
 */
PropertiesEditorDialog::PropertiesEditorDialog(wxWindow* parent, MRL::VariantPropertyTree &data,  MRL::PropertyPath &p)
    : PropertiesEditorDialogBase(parent),_loader(this->GetProperties()), _data(data), _path(p)
{

}

/*!
 * \brief PropertiesEditorDialog::~PropertiesEditorDialog
 */
PropertiesEditorDialog::~PropertiesEditorDialog()
{

}


/*!
 * \brief PropertiesEditorDialog::OnOk
 * \param event
 */
void PropertiesEditorDialog::OnOk(wxCommandEvent& /*event*/)
{

    EndModal(wxID_OK);
}
void PropertiesEditorDialog::OnNotes(wxCommandEvent& event)
{
}
void PropertiesEditorDialog::OnSettings(wxCommandEvent& event)
{
}
