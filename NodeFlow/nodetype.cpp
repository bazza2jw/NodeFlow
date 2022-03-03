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

#include "nodetype.h"
#include "nodeset.h"
#include "PropertiesEditorDialog.h"
#include "webproperties.h"
//
//
NODEFLOW::NodeType::NodeTypeMap NODEFLOW::NodeType::_map __attribute__ ((init_priority (110))); // map name to factory
NODEFLOW::NodeType::NodeTypeIdMap NODEFLOW::NodeType::_mapId __attribute__ ((init_priority (110))); // map id to factory
unsigned NODEFLOW::NodeType::_msgId = 100;
//
const char * NODEFLOW::NodeType::props[NODEFLOW::NumberConnectionType] =
{
    "String,%s,%s, , , ",
    "Bool,%s,%s,0,0,0",
    "Integer,%s,%s,0,-10000000,10000000",
    "Float,%s,%s,0.0,-10000000.0,10000000.0",
    "String,%s,%s, , , "
};
//
/*!
 * \brief calculate
 * \param ns
 * \param nodeId
 * \param input id
 * \param data calculated value
 * \return true on success
 */
bool NODEFLOW::NodeType::calculate(NodeSet &ns, unsigned nodeId, unsigned id, VALUE &data )
{
    try
    {
        // evaluate each of the outputs attached to this input
        NodePtr  &n = ns.findNode(nodeId);
        VALUE r;
        if(n)
        {
            ItemListPtr &il = n->inputs()[id];
            if(il)
            {
                for(auto i = il->begin(); i != il->end(); i++)
                {
                    EdgePtr &e = ns.findEdge(*i);
                    if(e)
                    {
                        VALUE d;
                        NodePtr &src = ns.findNode(e->from().node());
                        if(src)
                        {
                            NodeType *t = src->nodeType();
                            if(t->evaluate(ns,e->from().node(),e->from().id(),d))
                            {
                                // we have one value from one output connected to the input
                                processEvaluatedData(ns,n,id,d,r);
                            }
                        }
                    }
                }
            }
            data = r;
        }
    }
    CATCH_DEF
    return false;
}




/*!
 * \brief NODEFLOW::NodeType::evaluate
 * \param ns
 * \param nodeId
 * \param id output id
 * \param data
 * \return true on success
 */
bool NODEFLOW::NodeType::evaluate(NodeSet &ns, unsigned nodeId, unsigned id,  VALUE &data )
{
    try
    {
        // for the given output evaluate the inputs for the output
        NodePtr  &n = ns.findNode(nodeId);
        if(n)
        {
            if(n->calculatedData().find(id) != n->calculatedData().end())
            {
                data = n->calculatedData().at(id); // cached value
                return true;
            }
        }
    }
    CATCH_DEF
    return false;
}

/*!
 * \brief process
 * \param nodeId this node to process the signal
 * \param id this is the input index
 * \param data thhe data packet - note the const is important - if the packet mutates copy and modify the copy and post on the copy
 * \return true on success
 */
bool NODEFLOW::NodeType::process(NodeSet &/*ns*/, unsigned /*nodeId*/, unsigned /*id*/, const VALUE &/*data*/)
{
    return false;
}
/*!
 * \brief post
 * \param nodeId this is the source node
 * \param id this is the output index
 * \return true on success
 */
bool NODEFLOW::NodeType::post(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data )
{
    bool ret = false;
    try
    {
        NodePtr &n = ns.findNode(nodeId);
        if(n)
        {
            // get the list of edges attached to the output
            ItemListPtr &il = n->outputs()[id];
            if(il->size() > 0) // connected to anything ?
            {
                for(auto i = il->begin(); i != il->end(); i++)
                {
                    EdgePtr &e = ns.findEdge(*i);
                    if(e)
                    {
                        NodePtr &dn = ns.findNode(e->to().node());
                        if(dn)
                        {
                            NodeType *t = NodeType::find(dn->type()); // get the destination type
                            if(t)
                            {
                                // process each connection
                                ret |= t->process(ns,dn->id(),e->to().id(),data);
                            }
                        }
                    }
                }
            }
        }
    }
    CATCH_DEF

    return ret;
}

/*!
 * \brief NODEFLOW::NodeType::setupConnections
 */
void NODEFLOW::NodeType::setupConnections()
{
    // setup the inputs
    inputs().resize(1);
    inputs()[0] = Connection("in",Multiple);
    // set up the outputs
    outputs().resize(1);
    outputs()[0] = Connection("out",Multiple);
}

/*!
 * \brief NODEFLOW::NodeType::setup
 * default -
 */
void NODEFLOW::NodeType::setup()
{
    //
    setupConnections();
    //
    _layout.setInputCount(inputs().size());
    _layout.setOutputCount(outputs().size());

    int h =  NODE_TITLE_BAR_HEIGHT + NODE_RECT_HEIGHT_MARGIN ; // title
    h += CONNECTION_SPACE * ((inputs().size() > outputs().size())? inputs().size():outputs().size());
    wxRect r(0,0,NODE_RECT_WIDTH,h);
    _layout.setRect(r);
    //
    // set the connection positions
    for(int i = 0 ; i < inputs().size(); i++)
    {
        _layout.addInput(wxPoint(0,NODE_TITLE_BAR_HEIGHT + i *CONNECTION_SPACE),
                         inputs()[i].type());
    }

    for(int i = 0; i < outputs().size(); i++)
    {
        _layout.addOutput(wxPoint(NODE_RECT_WIDTH - CONNECTION_SIZE,NODE_TITLE_BAR_HEIGHT + i *CONNECTION_SPACE),
                          outputs()[i].type());
    }
}


/*!
 * \brief NODEFLOW::NodeType::properties
 * \param n
 * \param nodeId
 * \return true if OK
 */
bool NODEFLOW::NodeType::properties(wxWindow *parent, NodeSet &ns, unsigned nodeId)
{
    try
    {
        // property page
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
    }
    CATCH_DEF
    return false;
}
/*!
 * \brief NODEFLOW::NodeType::load
 * \param dlg
 * \param ns
 * \param p
 */
void NODEFLOW::NodeType::load(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
{
    dlg.loader().addStringProperty("Name","Name",ns.data().getValue<std::string>(p,"Name")); // field[0]
    dlg.loader().addBoolProperty("Enable Node","Enable",ns.data().getValue<bool>(p,"Enabled")); // field[1]
    wxString cs(ns.data().getValue<std::string>(p,"Colour"));
    wxColour c(cs);
    dlg.loader().addColourProperty("Node Colour","Colour",c); // field[2]

}
/*!
 * \brief NODEFLOW::NodeType::save
 * \param dlg
 * \param ns
 * \param p
 */
void NODEFLOW::NodeType::save(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
{
    wxVariant v = dlg.loader().fields()[0]->GetValue();
    ns.data().setValue(p,"Name",v.GetString().ToStdString());
    v = dlg.loader().fields()[1]->GetValue();
    ns.data().setValue(p,"Enabled",v.GetBool());
    wxColourProperty *c = static_cast<wxColourProperty *>(dlg.loader().fields()[2]);
    wxString s = "rgb" + c->GetValueAsString();
    ns.data().setValue(p,"Colour", s.ToStdString());

}

//
// Web properties dialog
//
#ifdef USE_WT

bool NODEFLOW::NodeType::properties(Wt::WWidget *parent, NodeSet &ns, unsigned nodeId)
{
    MRL::PropertyPath p;
    NodePtr &n = ns.findNode(nodeId);
    n->toPath(p);
    std::unique_ptr<NODEFLOW::WebPropertiesDialog> pd = std::make_unique<NODEFLOW::WebPropertiesDialog>(ns,n);
    NODEFLOW::WebDialogBase::showDialog<NODEFLOW::WebPropertiesDialog>(parent,pd);
    return true; // does not block
}
void NODEFLOW::NodeType::load(WebProperties *dlg,NodeSet &ns,MRL::PropertyPath p)
{
    dlg->addStringProperty("Name",ns.data().getValue<std::string>(p,"Name")); // field[0]
    dlg->addBoolProperty("Enable Node",ns.data().getValue<bool>(p,"Enabled")); // field[1]
    std::string cs = ns.data().getValue<std::string>(p,"Colour");
    dlg->addColourProperty("Node Colour",cs); // field[2]
}
void NODEFLOW::NodeType::save(WebProperties *dlg,NodeSet &ns,MRL::PropertyPath p)
{
    ns.data().setValue(p,"Name",dlg->getString(PropName));
    ns.data().setValue(p,"Enabled",dlg->getBool(PropEnable));
    std::string cs = dlg->getColour(PropColour);
    std::string s = "rgb" + cs;
    ns.data().setValue(p,"Colour", cs);
}



#endif
