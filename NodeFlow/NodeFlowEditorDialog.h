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
