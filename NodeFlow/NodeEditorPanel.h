#ifndef NODEEDITORPANEL_H
#define NODEEDITORPANEL_H
#include "NodeEditor.h"
#include "canvaswindows.h"

class NodeEditorPanel : public NodeEditorPanelBase
{
    wxTreeItemId _root;
    CanvasWindows * _canvas = nullptr;
    NODEFLOW::VALUE _value;
public:
    NodeEditorPanel(wxWindow* parent);
    virtual ~NodeEditorPanel();
public:
    CanvasWindows * canvas() const { return _canvas;}
    virtual void onClear(wxCommandEvent& event);
    virtual void onNew(wxCommandEvent& event);
    virtual void onOpen(wxCommandEvent& event);
    virtual void onRun(wxCommandEvent& event);
    virtual void onSave(wxCommandEvent& event);
    virtual void onSaveAs(wxCommandEvent& event);
    virtual void onStop(wxCommandEvent& event);
    virtual void onDragInit(wxTreeEvent& event);
protected:
    virtual void onStartSet(wxCommandEvent& event);
    virtual void onInject(wxCommandEvent& event);
    virtual void onTimer(wxTimerEvent& event);
    virtual void onEditRun(wxCommandEvent& event);
    virtual void onStep(wxCommandEvent& event);
    virtual void onTrigger(wxCommandEvent& event);
};
#endif // NODEEDITORPANEL_H
