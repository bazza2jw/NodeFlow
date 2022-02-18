#include "NodeEditorPanel.h"
#include "canvaswindows.h"
#include "../NodeFlow/nodetype.h"

/*!
 * \brief NodeEditorPanel::NodeEditorPanel
 * \param parent
 */
NodeEditorPanel::NodeEditorPanel(wxWindow* parent)
    : NodeEditorPanelBase(parent)
{
    _canvas = new CanvasWindows(GetSplitterRight(),1024,800,10);
    canvasSizer->Add(_canvas, 1, wxALL|wxEXPAND, WXC_FROM_DIP(5));
    // build the item tree
    _root = GetNodeTypes()->AddRoot("Types") ;
    //
    NODEFLOW::NodeType::NodeTypeMap &m = NODEFLOW::NodeType::map();
    std::set<const char *> classes;
    //
    for(auto i = m.begin(); i != m.end(); i++)
    {
        NODEFLOW::NodeType *t = i->second;
        if(t)
        {
            const char * c =  t->nodeClass();
            classes.insert(c);
        }
    }
    //
    std::map<std::string, wxTreeItemId> chdr;
    for(auto i = classes.begin(); i != classes.end(); i++)
    {
        std::string c(*i);
        chdr[c] = GetNodeTypes()->AppendItem (_root,c); // create the headers
    }
    //
    for(auto i = m.begin(); i != m.end(); i++)
    {
        NODEFLOW::NodeType *t = i->second;
        if(t)
        {
            std::string c(t->nodeClass());
            GetNodeTypes()->AppendItem (chdr[c],t->name());
        }
    }
    Connect(GetNodeTypes()->GetId(),wxEVT_COMMAND_TREE_BEGIN_DRAG,wxTreeEventHandler(NodeEditorPanel::onDragInit),NULL,this);
}
/*!
 * \brief NodeEditorPanel::~NodeEditorPanel
 */
NodeEditorPanel::~NodeEditorPanel()
{
    Disconnect(GetNodeTypes()->GetId(),wxEVT_COMMAND_TREE_BEGIN_DRAG,wxTreeEventHandler(NodeEditorPanel::onDragInit),NULL,this);
}

/*!
 * \brief NodeEditorPanel::onDragInit
 * \param event
 */
void NodeEditorPanel::onDragInit(wxTreeEvent& event)
{
    wxString s = GetNodeTypes()->GetItemText(event.GetItem());
    wxTextDataObject tdo(s);
    wxDropSource tds(tdo, GetNodeTypes());
    tds.DoDragDrop(wxDrag_CopyOnly);
}

/*!
 * \brief NodeEditorPanel::onNew
 * \param event
 */
void NodeEditorPanel::onNew(wxCommandEvent& event)
{
  _canvas->newFlow();
}
/*!
 * \brief NodeEditorPanel::onOpen
 * \param event
 */
void NodeEditorPanel::onOpen(wxCommandEvent& event)
{
    _canvas->open();
}
/*!
 * \brief NodeEditorPanel::onRun
 * \param event
 */
void NodeEditorPanel::onRun(wxCommandEvent& event)
{
}
/*!
 * \brief NodeEditorPanel::onSave
 * \param event
 */
void NodeEditorPanel::onSave(wxCommandEvent& event)
{
    _canvas->save();
}
/*!
 * \brief NodeEditorPanel::onSaveAs
 * \param event
 */
void NodeEditorPanel::onSaveAs(wxCommandEvent& event)
{
    _canvas->saveAs();
}
/*!
 * \brief NodeEditorPanel::onStop
 * \param event
 */
void NodeEditorPanel::onStop(wxCommandEvent& event)
{
}
void NodeEditorPanel::onClear(wxCommandEvent& event)
{
}
