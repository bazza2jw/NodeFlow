/*
 * Copyright (C) 2022 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */

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
    NodeSet & set()  {
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
    virtual const char * nodeClass() const { return "Node Sets";}

    /*!
     * \brief setupConnections
     */
    void setupConnections()
    {
        inputs().resize(0);
        outputs().resize(1);
        outputs()[0] = Connection("out",Multiple,Any);
    }
    bool step(NodeSet &,NodePtr &) { return true;}
    void trigger(NodeSet &ns, NodePtr &node )
    {
        post(ns,node->id(), 0,ns.inValue());
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
    virtual const char * nodeClass() const { return "Node Sets";}
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
            ns.setOutValue(data);
            return true;
        }
        return false;
    }
};


/*!
 * \brief The NodeSetType class
 * Sub flow functions
 */
class NodeSetType : public NodeType
{
public:
    /*!
     * \brief NodeSetType
     * \param s
     */
    NodeSetType(const std::string &s) : NodeType(s) {}
    virtual const char * nodeClass() const { return "Node Sets";}
    /*!
     * \brief create
     * \param i
     * \return
     */
    virtual Node * create(unsigned i)
    {
        return new SetNode(i,id());
    }

    /*!
     * \brief start
     * \param ns
     * \param node
     */
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
    /*!
     * \brief stop
     * \param ns
     * \param node
     */
    virtual void stop(NodeSet &ns,  NodePtr &node)
    {
        SetNode * n = static_cast<SetNode *>(node.get());
        n->set().stop();
    }

    /*!
     * \brief setupConnections
     */
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
        outputs()[0] = Connection("out",Multiple,Any);
    }
    /*!
     * \brief process
     * \param ns
     * \param nodeId
     * \param id
     * \param data
     * \return
     */
    bool process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data);
    /*!
     * \brief properties
     * \param parent
     * \param ns
     * \param nodeId
     * \return
     */
    bool properties(wxWindow * parent, NodeSet &ns, unsigned nodeId);
    /*!
     * \brief save
     * \param dlg
     * \param ns
     * \param p
     */
    void save(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p);
    /*!
     * \brief load
     * \param dlg
     * \param ns
     * \param p
     */
    void load(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p);

    /*!
     * \brief load
     * \param dlg
     * \param ns
     * \param p
     */
    void load(WebProperties *dlg,NodeSet &ns,MRL::PropertyPath p);
    /*!
     * \brief save
     * \param dlg
     * \param ns
     * \param p
     */
    void save(WebProperties *dlg,NodeSet &ns,MRL::PropertyPath p);


};
}
#endif // NODESETTYPE_H
