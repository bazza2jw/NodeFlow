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

#ifndef NODESET_H
#define NODESET_H
#include <MrlLib/mrllib.h>
#include <MrlLib/variantpropertytree.h>
#include <MrlLib/sqlitedb.h>
//
#include "node.h"
#include "nodetype.h"
#include "edge.h"
//
#include <functional>
#include <queue>


namespace NODEFLOW
{
#define NODE_SET_DIR "/usr/local/NodeFlow/sets/"

typedef std::function<void(NodePtr &)> NodeIteratorFunc;
typedef std::function<void(EdgePtr &)> EdgeIteratorFunc;
typedef std::queue<VALUE> VALUEQUEUE;

/*!
     * \brief The NodeSet class
     * Node /  Edge add and delete shoudl be done via the NodeSet not the maps
     */
class NodeSet
{
    //
    // Set of nodes
    //
    NodePtrMap _nodes;
    // set of edges
    EdgePtrMap _edges;
    //
    unsigned _edgeId = 100;
    unsigned _nodeId = 100;
    //
    NodePtr _null;
    //
    MRL::VariantPropertyTree _data;
    //
    VALUE _inValue; // value input to the set read by node set input nodes
    VALUEQUEUE _outValue; // written by node set output nodes

public:
    /*!
     * \brief NodeSet
     */
    NodeSet() {}
    /*!
     * \brief ~NodeSet
     */
    virtual ~NodeSet() {}
    /*!
     * \brief clear
     */
    void clear(); // clear everything
    /*!
     * \brief clearNodesSelected
     */
    void clearNodesSelected() // clear node selection flag
    {
        for(auto i = _nodes.begin(); i != _nodes.end(); i++)
        {
            NodePtr &n = i->second;
            n->setSelected(false);
        }
    }

    /*!
     * \brief clearEdgesSelected
     */
    void clearEdgesSelected() // clear edge selection flag
    {
        for(auto i = _edges.begin(); i != _edges.end(); i++)
        {
            EdgePtr &e = i->second;
            e->setSelected(false);
        }
    }
    /*!
     * \brief clearSelected
     */
    void clearSelected() // clear all selected items
    {
        clearNodesSelected();
        clearEdgesSelected();
    }
    /*!
     * \brief nodes
     * \return
     */
    NodePtrMap & nodes() {
        return  _nodes;
    }



    /*!
     * \brief edges
     * \return
     */
    EdgePtrMap & edges() {
        return _edges;
    }

    /*!
     * \brief toNodePath
     * \param id
     * \param path
     */
    void toNodePath(unsigned id, MRL::PropertyPath &path)
    {
        NodePtr & p = findNode(id);
        if(p)
        {
            p->toPath(path);
        }
    }
    //
     MRL::VariantPropertyTree & data() { return  _data;}
    /*!
     * \brief addNode of given type
     * \param s - either const std::string & or size_t
     */
     template<typename T>
    unsigned addNode(T s, int x = 0, int y = 0, unsigned id = 0)
    {
        try
        {
            NodeType *t = NodeType::find(s);
            if(t)
            {
                if(id == 0) id = _nodeId++;
                Node * n =  t->createNode(id);
                if(n)
                {
                    _nodes[id] = std::move(std::unique_ptr<Node>(n));
                    MRL::PropertyPath p;
                    p.push_back("Nodes");
                    p.push_back(n->sid());
                    data().setValue(p,"Id",id);
                    n->setLocation(x,y);
                    data().setValue(p,"X",x);
                    data().setValue(p,"Y",y);
                    std::string sn = "ID" + std::to_string(id);
                    data().setValue(p,"Name",sn);
                    n->save(*this);
                }
            }
        }
        CATCH_DEF
        return 0;
    }


    /*!
     * \brief deleteNode
     * \param id
     */
    void deleteNode(unsigned id)
    {
        NodePtr &n = findNode(id);
        if(n)
        {
            MRL::PropertyPath p;
            n->toPath(p);
            data().remove(p);
            disconnectNode(id);
            _nodes.erase(id);
        }
    }
    /*!
     * \brief find
     * \param id
     * \return
     */
    NodePtr & findNode(unsigned id)
    {
        auto i = _nodes.find(id);
        if(i != _nodes.end())
        {
            return i->second;
        }
        return _null;
    }
    /*!
     * \brief addConnect
     * \param nodeFrom
     * \param out
     * \param nodeTo
     * \param in
     */
    unsigned addConnect(unsigned nodeFrom, unsigned out, unsigned nodeTo, unsigned in);

    /*!
     * \brief connect
     * \param nodeFrom
     * \param out
     * \param nodeTo
     * \param in
     */
    void connect(unsigned id, unsigned nodeFrom, unsigned out, unsigned nodeTo, unsigned in);
    /*!
     * \brief disconnect
     * \param id
     */
    void disconnect(unsigned id); // remove an edge - remove from database remove from tree
    //
    // Edge database
    //
    /*!
     * \brief search
     * \param e
     */
    unsigned search(const Edge &e) // look for duplicate
    {
        for(auto i = _edges.begin(); i != _edges.end(); i++)
        {
            EdgePtr &j = i->second;
            if(e == *j) return i->first; // return the edge id
        }
        return 0;
    }
    /*!
     * \brief disconnectNode
     * \param id
     */
    void disconnectNode(unsigned id);

    EdgePtr _nullEdge;
    /*!
     * \brief findEdge
     * \param id
     * \return
     */
    EdgePtr & findEdge(unsigned id)
    {
        auto i = _edges.find(id);
        if(i != _edges.end()) return i->second;
        return _nullEdge;
    }
    /*!
     * \brief enumNodes
     * \param f
     */
    void enumNodes(const NodeIteratorFunc &f);
    /*!
     * \brief enumEdges
     * \param f
     */
    void enumEdges(const EdgeIteratorFunc &f);

    //
    virtual void step(const VALUE &in);
    virtual void start();
    virtual void stop();
    //
    virtual void load();
    virtual void save();

    //
    VALUE & inValue()  { return _inValue;}
    VALUEQUEUE & outValue()  { return _outValue;}
    void setInValue(const VALUE &v) { _inValue = v;}
    void setOutValue(const VALUE &v) { _outValue.push(v);}
    //

};
}
#endif // NODESET_H
