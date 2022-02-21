#include "NodeEditorPanel.h"
#include "canvaswindows.h"
#include "../NodeFlow/nodetype.h"
#include <MrlLib/mrllib.h>
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
    if(GetTimerStep()->IsRunning())
    {
        GetButtonRun()->SetLabelText("Run");
        GetTimerStep()->Stop();
    }
    else
    {
        // drive the flow from a timer
        GetTimerStep()->Start(GetTimerInterval()->GetValue());
        GetButtonRun()->SetLabelText("Stop");
        _canvas->nodeSet().start();
    }
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
/*!
 * \brief NodeEditorPanel::onClear
 * \param event
 */
void NodeEditorPanel::onClear(wxCommandEvent& /*event*/)
{
    GetListTrace()->Clear();
}
/*!
 * \brief NodeEditorPanel::onEditRun
 */
void NodeEditorPanel::onEditRun(wxCommandEvent& /*event*/)
{
    bool f = GetToggleEdit()->GetValue();
    GetButtonRun()->Enable(f);
    GetButtonStep()->Enable(f);
    GetButtonTrigger()->Enable(f);
    GetTimerInterval()->Enable(f);
    GetTimerStep()->Stop();
    _canvas->setEditMode(!f);

}
/*!
 * \brief NodeEditorPanel::onStep
 */
void NodeEditorPanel::onStep(wxCommandEvent& /*event*/)
{

    _canvas->nodeSet().step(_value);
    // now read the output queue and print it
    NODEFLOW::NodeSet &st = _canvas->nodeSet();
    NODEFLOW::VALUEQUEUE &q = st.outValue();
    while(q.size() > 0)
    {
        std::string s;
        MRL::jsonToString(q.front(), s);
        GetListTrace()->AppendString(s);
        q.pop();
    }

}
/*!
 * \brief NodeEditorPanel::onTrigger
 * \param event
 */
void NodeEditorPanel::onTrigger(wxCommandEvent& event)
{

}
/*!
 * \brief NodeEditorPanel::onTimer
 */
void NodeEditorPanel::onTimer(wxTimerEvent& /*event*/)
{
    wxCommandEvent e;
    onStep(e);
}
/*!
 * \brief NodeEditorPanel::onInject
 */
void NodeEditorPanel::onInject(wxCommandEvent& /*event*/)
{
     wxTextEntryDialog dlg(this, "Enter JSON Input", wxGetTextFromUserPromptStr, "{}");
     if(dlg.ShowModal() == wxID_OK)
     {
         std::string  s = dlg.GetValue().ToStdString();
         if(MRL::stringToJson(s,_value))
         {
            _canvas->nodeSet().setInValue(_value);
         }
     }
}
