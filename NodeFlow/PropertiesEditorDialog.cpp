
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
