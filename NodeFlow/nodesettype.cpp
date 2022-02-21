#include "nodesettype.h"
#include "NodeFlowEditorDialog.h"
#include "NodeEditorPanel.h"

/*!
 * \brief The NodeSetPropertiesEditorDialog class
 */
class NodeSetPropertiesEditorDialog : public PropertiesEditorDialog
{
public:
    NodeSetPropertiesEditorDialog(wxWindow* parent, MRL::VariantPropertyTree &data,  MRL::PropertyPath &p) :
        PropertiesEditorDialog(parent,data,p) {}
    void OnSettings(wxCommandEvent& /*event*/)
    {
        std::string f = data().getValue<std::string>(path(),"NodeSet");
        if(f[0] != '/')
        {
            f = NODE_SET_DIR + f; // add the default directory
        }
        NodeFlowEditorDialog dlg(this,f);
        dlg.editor()->canvas()->readSet(f);
        if(dlg.ShowModal() == wxID_OK)
        {
            dlg.editor()->canvas()->save();
        }
    }
};


/*!
 * \brief NODEFLOW::NodeSetType::properties
 * \param parent
 * \param ns
 * \param nodeId
 * \return true on success
 */
bool NODEFLOW::NodeSetType::properties(wxWindow * parent, NodeSet &ns, unsigned nodeId)
{
    MRL::PropertyPath p;
    NodePtr &n = ns.findNode(nodeId);
    n->toPath(p);
    NodeSetPropertiesEditorDialog dlg(parent,ns.data(),p);
    //
    load(dlg,ns,p);
    if(dlg.ShowModal() == wxID_OK)
    {
        save(dlg,ns,p);
        return true;
    }
    return false;
}

/*!
 * \brief NODEFLOW::NodeSetType::load
 * \param dlg
 * \param ns
 * \param p
 */
void NODEFLOW::NodeSetType::load(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
{
    dlg.loader().addStringProperty("Name","Name",ns.data().getValue<std::string>(p,"Name")); // field[0]
    dlg.loader().addBoolProperty("Enable Node","Enable",ns.data().getValue<bool>(p,"Enabled")); // field[1]
    dlg.loader().addStringProperty("Node Set","NodeSet",ns.data().getValue<std::string>(p,"NodeSet")); // field[2]
}

/*!
 * \brief NODEFLOW::NodeSetType::save
 * \param dlg
 * \param ns
 * \param p
 */
void NODEFLOW::NodeSetType::save(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
{
    wxVariant v = dlg.loader().fields()[0]->GetValue();
    ns.data().setValue(p,"Name",v.GetString().ToStdString());
    v = dlg.loader().fields()[1]->GetValue();
    ns.data().setValue(p,"Enabled",v.GetBool());
    v = dlg.loader().fields()[2]->GetValue();
    ns.data().setValue(p,"NodeSet",v.GetString().ToStdString());
}

/*!
 * \brief NODEFLOW::NodeSetType::process
 * \param ns
 * \param nodeId
 * \param id
 * \param data
 * \return true on success / posted
 */
bool NODEFLOW::NodeSetType::process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data)
{
    NodePtr &n = ns.findNode(nodeId);
    if(n && n->enabled())
    {
        switch(id)
        {
        case 0:
        {
            SetNode * s = static_cast<SetNode *>(n.get());
            s->set().step(data);
            VALUEQUEUE &q = s->set().outValue(); // there may be zero or more output values - topics can be different
            while(q.size() > 0)
            {
                post(ns,nodeId,Output,q.front()); // forward item on queue
                q.pop();
            }
            true;
        }
        default:
            break;
        }
    }
    return false;
}
