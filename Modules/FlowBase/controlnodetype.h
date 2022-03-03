#ifndef CONTROLNODETYPE_H
#define CONTROLNODETYPE_H
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
#include <MrlLib/mrllib.h>
#include "../../NodeFlow/nodetype.h"
#include "../../NodeFlow/nodeset.h"
#include "../../NodeFlow/fparser.hh"
#include "../../NodeFlow/PropertiesEditorDialog.h"


namespace NODEFLOW
{
    /*!
     * \brief The IfElseNodeType class
     * this is packet switching node based on an expresion compare
     */

    class IfElseNodeType : public NodeType
    {
     protected:
        /*!
         * \brief The ParserWithConsts class
         */
        class ParserWithConsts: public FunctionParser
        {
        public:
            ParserWithConsts()
            {
                AddConstant("pi", 3.14159265358979323846);
                AddConstant("e", 2.71828182845904523536);
            }
        };
        ParserWithConsts _parser;
        std::vector<double> _vars;

    public:
        enum
        {
            Input = 0, TrueOutput = 0, ElseOutput = 1
        };
        /*!
         * \brief IfElseNodeType
         * \param s
         */
        IfElseNodeType(const std::string &s = "IfElse") : NodeType(s),_vars(1){}
        /*!
         * \brief nodeClass
         * \return
         */
        virtual const char * nodeClass() const { return "Control";}
        //
        /*!
         * \brief compile
         * \param func
         * \param args
         * \return
         */
        bool compile(const std::string &func, const std::string &args = "a")
        {
            _vars.resize(1);
            return _parser.Parse(func,args) == -1;
        }
        /*!
         * \brief start
         * \param ns
         * \param n
         */
        virtual void start(NodeSet &ns,  NodePtr &n)
        {
            MRL::PropertyPath p;
            n->toPath(p);
            std::string f =  ns.data().getValue<std::string>(p,"Function");
        }

        /*!
         * \brief process
         * \param ns
         * \param nodeId
         * \param id
         * \param data
         * \return true if processed
         */
        bool process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data)
        {
            NodePtr &n = ns.findNode(nodeId);
            if(n && n->enabled())
            {
                switch(id)
                {
                case Input:
                {
                    _vars[0] = data[DATA_PAYLOAD].asDouble();
                    double r = _parser.Eval(_vars.data());
                    n->data()["OUT"] = std::floor(r);
                    if(r != 0)
                    {
                        // if true send the packet through the true output
                        return post(ns,nodeId,TrueOutput,data);
                    }
                    else {
                        // otherwise send out of the else output
                        return post(ns,nodeId,ElseOutput,data);
                    }
                }
                default:
                    break;
                }
            }
            return false;

        }
        /*!
         * \brief load
         * \param dlg
         * \param ns
         * \param p
         */
        virtual void load(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
        {
            NodeType::load(dlg,ns,p);
            dlg.loader().addStringProperty("Function","Function",ns.data().getValue<std::string>(p,"Function")); // field[2]
        }
        /*!
         * \brief save
         * \param dlg
         * \param ns
         * \param p
         */
        virtual void save(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
        {
            NodeType::save(dlg,ns,p);
            wxVariant v = dlg.loader().fields()[PropField1]->GetValue();
            std::string f = v.GetString().ToStdString();
            ns.data().setValue(p,"Function",f);
        }
        /*!
         * \brief setupConnections
         */
        void setupConnections()
        {
            inputs().resize(1);
            inputs()[0] = Connection("in",Multiple,Float);
            // set up the outputs
            outputs().resize(2);
            outputs()[0] = Connection("out",Multiple,Float);
            outputs()[1] = Connection("else",Multiple,Float);
        }
        /*!
         * \brief setup
         */
        void setup()
        {
            setupConnections();
            //
            nodeLayout().setInputCount(inputs().size());
            nodeLayout().setOutputCount(outputs().size());
            //
            int h =  NODE_TITLE_BAR_HEIGHT + NODE_RECT_HEIGHT_MARGIN ; // title
            h += CONNECTION_SPACE * ((inputs().size() > outputs().size())? inputs().size():outputs().size());
            wxRect r(0,0,NODE_RECT_WIDTH,h);
            nodeLayout().setRect(r);
            //
            // set the connection positions top of node
            nodeLayout().addInput(wxPoint(0,CONNECTION_SIZE));
            nodeLayout().addOutput(wxPoint(NODE_RECT_WIDTH - CONNECTION_SIZE,NODE_TITLE_BAR_HEIGHT)); // if true message goes this way
            nodeLayout().addOutput(wxPoint(NODE_RECT_WIDTH/2,h - CONNECTION_SIZE )); // the else goes on the bottom
        }


   };

    /*!
     * \brief The ElseIfNodeType class
     *  much the same as ifelse but the input is at the top easier for linking together with an if else , hence chaining
     */
    class ElseIfNodeType : public IfElseNodeType
    {
    public:
        /*!
         * \brief ElseIfNodeType
         * \param s
         */
        ElseIfNodeType(const std::string &s) : IfElseNodeType(s){}
        /*!
         * \brief nodeClass
         * \return
         */
        virtual const char * nodeClass() const { return "Control";}
        /*!
         * \brief setup
         */
        void setup()
        {
            setupConnections();
            //
            nodeLayout().setInputCount(inputs().size());
            nodeLayout().setOutputCount(outputs().size());
            //
            int h =  NODE_TITLE_BAR_HEIGHT + NODE_RECT_HEIGHT_MARGIN ; // title
            h += CONNECTION_SPACE * ((inputs().size() > outputs().size())? inputs().size():outputs().size());
            wxRect r(0,0,NODE_RECT_WIDTH,h);
            nodeLayout().setRect(r);
            //
            // set the connection positions top of node
            nodeLayout().addInput(wxPoint(0,0));
            nodeLayout().addOutput(wxPoint(NODE_RECT_WIDTH - CONNECTION_SIZE,NODE_TITLE_BAR_HEIGHT)); // if true message goes this way
            nodeLayout().addOutput(wxPoint(NODE_RECT_WIDTH/2,h - CONNECTION_SIZE )); // the else goes on the bottom
        }
    };



}
#endif // CONTROLNODETYPE_H
