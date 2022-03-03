/*
 * Copyright (C) 2022 -  B. J. Hill
 *
 * This file is part of NodeFlow. NodeFlow C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */

#include "node.h"
#include "nodetype.h"
#include "nodeset.h"

/*!
 * \brief NODEFLOW::Node::Node
 * \param id
 * \param type
 */
NODEFLOW::Node::Node(unsigned id , size_t type ) : _type(type)
{
    setId(id); // set up string id too
}

/*!
 * \brief NODEFLOW::Node::layout
 * \return
 */
const NODEFLOW::NodeLayout & NODEFLOW::Node::layout()
{
    static NodeLayout _null;
    NodeType *t = nodeType();
    if(t)
    {
        return t->nodeLayout(id());
    }
    return _null;
}

/*!
 * \brief NODEFLOW::Node::nodeType
 * \return
 */
NODEFLOW::NodeType * NODEFLOW::Node::nodeType() const
{
    return  NodeType::find(type());
}


/*!
 * \brief NODEFLOW::Node::load
 * \param s
 */
void NODEFLOW::Node::load(NodeSet &s)
{
    // load from the tree in a node set
    MRL::VariantPropertyTree &t = s.data();
    MRL::PropertyPath p;
    toPath(p); // location of node data
    _location.x = t.getValue<int>(p,"X");
    _location.y = t.getValue<int>(p,"Y");
    std::string cs = t.getValue<std::string>(p,"Colour"); // configured colour
    wxColour c(cs);
    _colour = c;
    _enabled= t.getValue<bool>(p,"Enabled"); // configured enabled state
}
/*!
 * \brief NODEFLOW::Node::save
 * \param s
 */
void NODEFLOW::Node::save(NodeSet &s)
{
    MRL::VariantPropertyTree &t = s.data();
    NodeType * nt = nodeType();
    if(nt)
    {
        MRL::PropertyPath p;
        toPath(p); // location of node data
        t.setValue(p,"Id",id());
        t.setValue(p,"Type",_type);
        t.setValue(p,"Typename",nt->name());
        t.setValue(p,"X",_location.x);
        t.setValue(p,"Y",_location.y);
        t.setValue(p,"Enabled",_enabled);
    }
}

/*!
 * \brief NODEFLOW::Node::getNodeEdges
 * \param edgeDrawSet
 */
void NODEFLOW::Node::getNodeEdges(EdgeIdSet &edgeDrawSet)
{
        // draw the connected edges
        for(auto i = inputs().begin(); i != inputs().end(); i++)
        {
            NODEFLOW::ItemListPtr &il = *i;
            if(il)
            {
                for(auto j = il->begin(); j != il->end(); j++)
                {
                    edgeDrawSet.insert(*j); // only draw edge once
                }
            }
        }

        for(auto i = outputs().begin(); i != outputs().end(); i++)
        {
            NODEFLOW::ItemListPtr &il = *i;
            if(il)
            {
                for(auto j = il->begin(); j != il->end(); j++)
                {
                    edgeDrawSet.insert(*j);
                }
            }
        }
}

