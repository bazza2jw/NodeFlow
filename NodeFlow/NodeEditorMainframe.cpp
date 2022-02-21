#include "NodeEditorMainframe.h"
#include <wx/app.h>
#include "NodeEditorChild.h"
/*!
 * \brief NodeEditorMainframe::NodeEditorMainframe
 * \param parent
 */
NodeEditorMainframe::NodeEditorMainframe(wxWindow* parent)
    : NodeEditorMainframeBase(parent)
{

}

NodeEditorMainframe::~NodeEditorMainframe()
{
}

void NodeEditorMainframe::onExit(wxCommandEvent& event)
{
    wxApp::GetInstance()->ExitMainLoop();
}
void NodeEditorMainframe::onNew(wxCommandEvent& event)
{
    // add a new page
    NodeEditorChild *w = new NodeEditorChild(this);
    w->Show();
}
