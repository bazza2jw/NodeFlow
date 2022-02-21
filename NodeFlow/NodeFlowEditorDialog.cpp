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
