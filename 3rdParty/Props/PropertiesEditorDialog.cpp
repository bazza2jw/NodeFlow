/*!
 *
 *  Author:  B. J. Hill
 *  (c)  Air Monitors Limited 2017 -
 */
#include "PropertiesEditorDialog.h"
#include "../AmpiCoreWindows/ObjectNotesDialog.h"

/*!
 * \brief PropertiesEditorDialog::PropertiesEditorDialog
 * \param parent
 * \param p
 */
PropertiesEditorDialog::PropertiesEditorDialog(wxWindow* parent, const HDB::HdbPath &p)
    : PropertiesEditorDialogBase(parent), PropertiesHelper(m_properties), _path(p)
{
}

/*!
 * \brief PropertiesEditorDialog::~PropertiesEditorDialog
 */
PropertiesEditorDialog::~PropertiesEditorDialog()
{

}

/*!
 * \brief PropertiesEditorDialog::OnNotes
 * \param event
 */
void PropertiesEditorDialog::OnNotes(wxCommandEvent& /*event*/)
{
    // open notes editor
    ObjectNotesDialog dlg(this, _path);
    dlg.ShowModal();
}
/*!
 * \brief PropertiesEditorDialog::OnOk
 * \param event
 */
void PropertiesEditorDialog::OnOk(wxCommandEvent& /*event*/)
{
    save(_path); // save properties to file
    EndModal(wxID_OK);
}
/*!
 * \brief PropertiesEditorDialog::OnSettings
 * \param event
 */
void PropertiesEditorDialog::OnSettings(wxCommandEvent& /*event*/)
{
    // derived classes handle settings / additional options
    handleSettings(this,_path);
}
