#ifndef VALUENODETYPE_H
#define VALUENODETYPE_H
#include "../../NodeFlow/nodetype.h"
#include "../../NodeFlow/nodeset.h"

#include <wx/timer.h>
#include "../../NodeFlow/PropertiesEditorDialog.h"

namespace NODEFLOW
{
//
// property edit cannot be templated elegantly
//
// value source
//
    template <typename T>
    class ValueNodeType : public NodeType
    {
    public:
        ValueNodeType(const std::string &s) : NodeType(s) {}
        void trigger(NodeSet &ns, NodePtr &n )
        {
            if(n && n->enabled())
            {
                MRL::VariantMap &m = n->data();
                if(m.find("Value") != m.end()) // get the value parameter
                {
                    T v = MRL::valueToType<T>(m["Value"]);
                    VALUE d(v);
                    post(ns,n->id(),0,d);
                }
            }
        }

        void setupConnections()
        {
            inputs().resize(0);
            // set up the outputs
            outputs().resize(_out);
            outputs()[0] = Connection("out",Multiple);
        }

    };

    class IntegerNodeType  : ValueNodeType<int>
    {
    public:
        IntegerNodeType() : ValueNodeType<int>("Integer") {}
        virtual bool properties(wxWindow * parent,NodeSet &ns, unsigned nodeId)
        {
            MRL::PropertyPath p;
            NodePtr &n = ns.findNode(nodeId);
            n->toPath(p);
            PropertiesEditorDialog dlg(parent,ns.data(),p);
        }
    };

    class DoubleNodeType  : ValueNodeType<double>
    {
    public:
        DoubleNodeType() : ValueNodeType<int>("Double") {}
        virtual bool properties(wxWindow * parent,NodeSet &n, unsigned nodeId);
    };

    class StringNodeType  : ValueNodeType<std::string>
    {
    public:
        StringNodeType() : ValueNodeType<std::string>("String") {}
        virtual bool properties(wxWindow * parent,NodeSet &n, unsigned nodeId);
    };

    class BoolNodeType  : ValueNodeType<bool>
    {
    public:
        BoolNodeType() : ValueNodeType<int>("Bool") {}
        virtual bool properties(wxWindow * parent,NodeSet &n, unsigned nodeId);
    };

    class TimerNodeType  : ValueNodeType<int>
    {
        wxStopWatch _timer;
    public:
        TimerNodeType() : ValueNodeType<int>("Timer")
        {
            _timer.Start();
        }

        void trigger(NodeSet &ns, NodePtr &n )
        {
            try
            {
                if(n && n->enabled())
                {
                    MRL::VariantMap &m = n->data();
                    if(m.find("Value") != m.end()) // get the value parameter
                    {
                        int i = MRL::valueToType<int>(m["Interval"]);
                        if(_timer.TimeInMicro()/1000 > i)
                        {
                            T v = MRL::valueToType<T>(m["Value"]);
                            VALUE d(v);
                            post(ns,n->id(),0,d);
                            _timer.Start();
                        }
                    }
                }
            }
            catch(...)
            {
                n->setEnabled(false);
            }
        }


        virtual bool properties(wxWindow * parent,NodeSet &n, unsigned nodeId);
    };




}
#endif // VALUENODETYPE_H
