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

#include "valuenodetype.h"
#include "NodeFlow/nodeset.h"
#include "NodeFlow/PropertiesEditorDialog.h"
#include <cstdlib>
#include <NodeFlow/webproperties.h>
/*!
 * \brief The IntegerNodeType class
 */
class IntegerNodeType  : public  NODEFLOW::ValueNodeType<int,NODEFLOW::Integer>
{
public:
    IntegerNodeType(const std::string &s) : ValueNodeType<int,NODEFLOW::Integer>(s) {}
};
/*!
 * \brief The DoubleNodeType class
 */
class DoubleNodeType  :   public  NODEFLOW::ValueNodeType<double,NODEFLOW::Float>
{
public:
    DoubleNodeType(const std::string &s) : NODEFLOW::ValueNodeType<double,NODEFLOW::Float>(s) {}
};

/*!
 * \brief The DoubleNodeType class
 */
class RandomNodeType  :   public  NODEFLOW::ValueNodeType<double,NODEFLOW::Float>
{
public:
    RandomNodeType(const std::string &s) : NODEFLOW::ValueNodeType<double,NODEFLOW::Float>(s) {}

    virtual bool process(NODEFLOW::NodeSet &ns, unsigned nodeId, unsigned /*id*/, const NODEFLOW::VALUE &data)
    {
        NODEFLOW::NodePtr &n = ns.findNode(nodeId);
        if(n && n->enabled())
        {
            MRL::PropertyPath p;
            n->toPath(p);
            int v = ns.data().getValue<int>(p,"Value");
            int r = std::rand();
            if(v > 1) r = r % v;
            NODEFLOW::VALUE result;
            setValueData(data,double(r),result);
            return post(ns,nodeId,0,result);
        }
        return false;
    }


    virtual void load(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::load(dlg,ns,p);
        dlg.loader().addIntProperty("Value","Value",ns.data().getValue<int>(p,"Value"));
    }
    virtual void save(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::save(dlg,ns,p);
        ns.data().setValue(p,"Value",dlg.loader().get<int>(NODEFLOW::PropField1));
    }

    void load(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::load(dlg,ns,p);
        dlg->addProp("Value",ns.data().getValue<int>(p,"Value"));
    }

    void save(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::save(dlg,ns,p);
        int v;
        dlg->get(NODEFLOW::PropField1,v);
        ns.data().setValue(p,"Value",v);
    }

};


/*!
 * \brief The StringNodeType class
 */
class StringNodeType  :  public  NODEFLOW::ValueNodeType<std::string,NODEFLOW::String>
{
public:
    StringNodeType(const std::string &s) : NODEFLOW::ValueNodeType<std::string,NODEFLOW::String>(s) {}
};
/*!
 * \brief The BoolNodeType class
 */
class BoolNodeType  :  public NODEFLOW::ValueNodeType<bool,NODEFLOW::Bool>
{
public:
    BoolNodeType(const std::string &s) : ValueNodeType<bool,NODEFLOW::Bool>(s) {}
};
/*!
 * \brief The TimerNodeType class
 */
class TimerNodeType  :  public NODEFLOW::NodeType
{
    class TimerNode : public NODEFLOW::Node
    {
        wxStopWatch _timer; // timer for node
        int _interval = 1000;
    public:
        /*!
         * \brief TimerNode
         * \param id
         * \param type
         */
        TimerNode(unsigned id = 0, size_t type = 0) : NODEFLOW::Node(id,type)
        {

        }
        /*!
        * \brief load
        * \param s
        */
        void load(NODEFLOW::NodeSet &ns)
        {
            MRL::PropertyPath p;
            toPath(p);
            NODEFLOW::Node::load(ns);
            _interval = ns.data().getValue<int>(p,"Value");
            _timer.Start();
        }
        /*!
         * \brief ticked
         * \return
         */
        bool ticked()
        {
            if(_timer.TimeInMicro()/1000 > _interval)
            {
                _timer.Start();
                return true;
            }
            return false;
        }

    };

public:
    /*!
     * \brief TimerNodeType
     * \param s
     */
    TimerNodeType(const std::string &s) : NodeType(s)
    {
    }

    /*!
     * \brief setupConnections
     */
    void setupConnections() // this is a trigger source only
    {
        inputs().resize(0);
        // set up the outputs
        outputs().resize(1);
        outputs()[0] = NODEFLOW::Connection("out",NODEFLOW::Multiple,NODEFLOW::Bool);
    }

    /*!
     * \brief start
     * \param ns
     * \param node
     * \return
     */
    void  start(NODEFLOW::NodeSet &ns, NODEFLOW::NodePtr &node)
    {
        NODEFLOW::NodeType::start(ns,node);
        TimerNode * n = static_cast<TimerNode *>(node.get());
        if(n)
        {
            n->load(ns);
        }
    }

    /*!
     * \brief create
     * \param i
     * \return
     */
    virtual NODEFLOW::Node * create(unsigned i)
    {
        return new TimerNode(i,id());
    }

    /*!
     * \brief step
     * \param ns
     * \param node
     * \return
     */
    bool step(NODEFLOW::NodeSet &ns, NODEFLOW::NodePtr &node)
    {
        TimerNode * n = static_cast<TimerNode *>(node.get());
        return n->ticked();
    }

    /*!
     * \brief trigger
     * \param ns
     * \param n
     */
    void trigger(NODEFLOW::NodeSet &ns, NODEFLOW::NodePtr &n )
    {
        try
        {
            if(n && n->enabled())
            {
                NODEFLOW::VALUE d;
                setValueData(std::string("Timer"), true, d);
                post(ns,n->id(),0,d);
            }
        }
        catch(...)
        {
            n->setEnabled(false);
        }
    }

    /*!
     * \brief load
     * \param dlg
     * \param ns
     * \param p
     */
    virtual void load(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::load(dlg,ns,p);
        dlg.loader().addIntProperty("Interval (ms)","Value",ns.data().getValue<int>(p,"Value"));
    }
    /*!
     * \brief save
     * \param dlg
     * \param ns
     * \param p
     */
    virtual void save(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::save(dlg,ns,p);
        wxAny v = dlg.loader().fields()[NODEFLOW::PropField1]->GetValue();
        ns.data().setValue(p,"Value",v.As<int>());
    }
    /*!
     * \brief load
     * \param dlg
     * \param ns
     * \param p
     */
    void load(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::load(dlg,ns,p);
        dlg->addProp("Interval(ms)",ns.data().getValue<int>(p,"Value"));
    }

    /*!
     * \brief save
     * \param dlg
     * \param ns
     * \param p
     */
    void save(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        NodeType::save(dlg,ns,p);
        int v;
        dlg->get(NODEFLOW::PropField1,v);
        ns.data().setValue(p,"Value",v);
    }

};

/*!
 * \brief addValueNodes
 */
void addValueNodes()
{
    NODEFLOW::NodeType::addType<TimerNodeType>("Timer");
    NODEFLOW::NodeType::addType<BoolNodeType>("Bool");
    NODEFLOW::NodeType::addType<StringNodeType>("String");
    NODEFLOW::NodeType::addType<DoubleNodeType>("Double");
    NODEFLOW::NodeType::addType<IntegerNodeType>("Integer");
    NODEFLOW::NodeType::addType<RandomNodeType>("Random");
    //
    NODEFLOW::NodeType::addType<NODEFLOW::GlobalBoolNodeType>("GlobalBool");
    NODEFLOW::NodeType::addType<NODEFLOW::GlobalIntegerNodeType>("GlobalInteger");
    NODEFLOW::NodeType::addType<NODEFLOW::GlobalFloatNodeType>("GlobalFloat");
    NODEFLOW::NodeType::addType<NODEFLOW::GlobalStringNodeType>("GlobalString");
    //
}
