#ifndef NODEEDITORPANEL_H
#define NODEEDITORPANEL_H
#include "NodeEditor.h"
#include "canvaswindows.h"

class NodeEditorPanel : public NodeEditorPanelBase
{
    wxTreeItemId _root;
    CanvasWindows * _canvas = nullptr;
public:
    NodeEditorPanel(wxWindow* parent);
    virtual ~NodeEditorPanel();
public:
    virtual void onClear(wxCommandEvent& event);
    virtual void onNew(wxCommandEvent& event);
    virtual void onOpen(wxCommandEvent& event);
    virtual void onRun(wxCommandEvent& event);
    virtual void onSave(wxCommandEvent& event);
    virtual void onSaveAs(wxCommandEvent& event);
    virtual void onStop(wxCommandEvent& event);
    virtual void onDragInit(wxTreeEvent& event);
};
#endif // NODEEDITORPANEL_H
