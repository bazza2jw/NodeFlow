#ifndef NODESETTYPE_H
#define NODESETTYPE_H
#include "nodetype.h"
#include "nodeset.h"
#include "node.h"
#include "PropertiesEditorDialog.h"
namespace NODEFLOW
{
/*!
     * \brief The SetNode class
     * This holds a node set in effect a sub flow
     */

class SetNode : public Node {
    NodeSet _set;
public:
    SetNode(unsigned id = 0, size_t type = 0) : Node(id,type) {}
    NodeSet & set() const {
        return  _set;
    }
};



/*!
 * \brief The NodeSetInputNodeType class
 */
class NodeSetInputNodeType : public NodeType
{
public:
    /*!
     * \brief BaseNodeType
     * \param n
     * \param nIn
     * \param nOut
     */
    NodeSetInputNodeType(const std::string &n)
        : NodeType(n) {}
    /*!
     * \brief setupConnections
     */
    void setupConnections()
    {
        inputs().resize(0);
        outputs().resize(1);
        outputs()[0] = Connection("out",Multiple,Any);
    }

    void trigger(NodeSet &ns, NodePtr &node )
    {
        SetNode *s = static_cast<SetNode *>(node.get());
        post(ns,node->id(), 0,s->set().inValue());
    }

};

/*!
 * \brief The NodeSetOutputNodeType class
 */
class NodeSetOutputNodeType : public NodeType
{
public:
    /*!
     * \brief BaseNodeType
     * \param n
     * \param nIn
     * \param nOut
     */
    NodeSetOutputNodeType(const std::string &n)
        : NodeType(n) {}
    /*!
     * \brief setupConnections
     */
    void setupConnections()
    {
        inputs().resize(1);
        outputs().resize(0);
        inputs()[0] = Connection("in",Multiple,Any);
    }
    virtual bool process(NodeSet &ns, unsigned nodeId, unsigned /*id*/, const VALUE &data)
    {
        NodePtr &n = ns.findNode(nodeId);
        if(n && n->enabled())
        {
            SetNode *s = static_cast<SetNode *>(n.get());
            s->set().setOutValue(data);
            return true;
        }
    }
};


/*!
 * \brief The NodeSetType class
 * Sub flow functions
 */
class NodeSetType : public NodeType
{
public:
    NodeSetType(const std::string &s) : NodeType(s) {}
    virtual Node * create(unsigned i)
    {
        return new SetNode(i,id());
    }


    virtual void start(NodeSet &ns,  NodePtr &node)
    {
        MRL::PropertyPath p;
        node->toPath(p);
        std::string f = ns.data().getValue<std::string>(p,"NodeSet");
        SetNode * n = static_cast<SetNode *>(node.get());
        n->set().clear();
        if(n->set().data().load(f))
        {
            n->set().load();
            n->set().start();
        }
    }

    virtual void stop(NodeSet &ns,  NodePtr &node)
    {
        SetNode * n = static_cast<SetNode *>(node.get());
        n->set().stop();
    }

    virtual void step(NodeSet &ns,  NodePtr &node)
    {
        SetNode * n = static_cast<SetNode *>(node.get());
        n->set().step(); // drive the sub flow step
    }

    void setupConnections()
    {
        //
        // there is one flow input and one output to a sub flow - can split streams by topic
        //
        inputs().resize(1);
        //
        inputs()[0] = Connection("in",Multiple,Any);
        //
        // set up the outputs
        outputs().resize(1);
        outputs()[0] = Connection("out",Multiple,Float);
    }



    virtual bool properties(wxWindow * parent,NodeSet &ns, unsigned nodeId)
    {
        MRL::PropertyPath p;
        NodePtr &n = ns.findNode(nodeId);
        n->toPath(p);
        PropertiesEditorDialog dlg(parent,ns.data(),p);
        //
        load(dlg,ns,p);
        if(dlg.ShowModal() == wxID_OK)
        {
            save(dlg,ns,p);
            return true;
        }
        return false;
    }

    virtual void load(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
    {
        dlg.loader().addStringProperty("Name","Name",ns.data().getValue<std::string>(p,"Name")); // field[0]
        dlg.loader().addBoolProperty("Enable Node","Enable",ns.data().getValue<bool>(p,"Enabled")); // field[1]
        dlg.loader().addStringProperty("Node Set","NodeSet",ns.data().getValue<std::string>(p,"NodeSet")); // field[2]
    }

    virtual void save(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
    {
        wxVariant v = dlg.loader().fields()[0]->GetValue();
        ns.data().setValue(p,"Name",v.GetString().ToStdString());
        v = dlg.loader().fields()[1]->GetValue();
        ns.data().setValue(p,"Enabled",v.GetBool());
        v = dlg.loader().fields()[2]->GetValue();
        ns.data().setValue(p,"NodeSet",v.GetString().ToStdString());
    }


    virtual bool process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data)
    {
        NodePtr &n = ns.findNode(nodeId);
        if(n && n->enabled())
        {
            switch(id)
            {
            case 0:
            {
                VALUE result;
                SetNode * s = static_cast<SetNode *>(n.get());
                s->set().step(data,result);
                return post(ns,nodeId,Output,result);
            }
            default:
                break;
            }
        }
        return false;
    }
};
}
#endif // NODESETTYPE_H
