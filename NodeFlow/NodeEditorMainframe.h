#ifndef NODEEDITORMAINFRAME_H
#define NODEEDITORMAINFRAME_H
#include "NodeEditor.h"
#include "canvaswindows.h"

class NodeEditorMainframe : public NodeEditorMainframeBase
{
    CanvasWindows * _canvas = nullptr;
public:
    NodeEditorMainframe(wxWindow* parent);
    virtual ~NodeEditorMainframe();
public:
    virtual void onNew(wxCommandEvent& event);
    virtual void onExit(wxCommandEvent& event);
};
#endif // NODEEDITORMAINFRAME_H
