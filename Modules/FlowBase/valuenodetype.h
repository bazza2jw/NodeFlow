#ifndef VALUENODETYPE_H
#define VALUENODETYPE_H
#include "NodeFlow/nodetype.h"
#include "NodeFlow/nodeset.h"
#include <wx/timer.h>
#include "NodeFlow/PropertiesEditorDialog.h"
#include "NodeFlow/webproperties.h"

namespace NODEFLOW
{
//


// property edit cannot be templated elegantly
//
// value source
//
    template <typename T, ConnectionType outType = Any>
/*!
     * \brief The ValueNodeType class
     */

    class ValueNodeType : public NodeType
    {
    public:
        /*!
         * \brief ValueNodeType
         * \param s
         */
        ValueNodeType(const std::string &s) : NodeType(s) {}

        virtual const char * nodeClass() const { return "Source Values";}


        virtual bool process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data)
        {
            NodePtr &n = ns.findNode(nodeId);
            if(n && n->enabled())
            {
                MRL::PropertyPath p;
                n->toPath(p);
                T v = ns.data().getValue<T>(p,"Value");
                VALUE result;
                setValueData(data,v,result);
                return post(ns,nodeId,Output,result);
            }
            return false;
        }

        void setupConnections()
        {
            inputs().resize(1);
            inputs()[0] =  Connection("trig",Multiple,Bool);
            // set up the outputs
            outputs().resize(1);
            outputs()[0] = Connection("out",Multiple,outType);
        }


        virtual void load(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
        {
            NodeType::load(dlg,ns,p);
            dlg.loader().addProp("Value","Value",ns.data().getValue<T>(p,"Value"));
        }
        virtual void save(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
        {
            NodeType::save(dlg,ns,p);
            T v = dlg.loader().get<T>(NODEFLOW::PropField1);
            ns.data().setValue(p,"Value",v);
        }

        void load(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
        {
            NodeType::load(dlg,ns,p);
            dlg->addProp("Value",ns.data().getValue<T>(p,"Value"));
        }

        void save(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
        {
            NodeType::save(dlg,ns,p);
            T v;
            dlg->get(NODEFLOW::PropField1,v);
            ns.data().setValue(p,"Value",v);
        }

    };



    template <typename T, ConnectionType outType = Any>
    /*!
     * \brief The GlobalValueNodeType class
     * Sets the value in the payload to a global entry in the node sets property tree and passes the packet through
     * These globals can ge retrived as required by other nodes
     * This can be useful things like temperature measurements
     */

    class GlobalValueNodeType : public NodeType
    {
    public:
        /*!
         * \brief ValueNodeType
         * \param s
         */
        GlobalValueNodeType(const std::string &s) : NodeType(s) {}

        virtual const char * nodeClass() const { return "Global Values";}

        virtual void start(NodeSet &ns,  NodePtr &n)
        {
            MRL::PropertyPath p;
            n->toPath(p);
            std::string t = ns.data().getValue<std::string>(p,"GlobalName");
            n->data()["GLOBALNAME"] = t;
        }


        virtual bool process(NodeSet &ns, unsigned nodeId, unsigned /*id*/, const VALUE &data)
        {
            NodePtr &n = ns.findNode(nodeId);
            if(n && n->enabled())
            {
                MRL::PropertyPath p;
                p.push_back("GLOBAL");
                T v = data[DATA_PAYLOAD].as<T>();
                ns.data().setValue(p,n->data()["GLOBALNAME"].asString(),v); // set the global value in the data tree
                return post(ns,nodeId,Output,data);
            }
            return false;
        }

        void setupConnections()
        {
            inputs().resize(1);
            inputs()[0] =  Connection("in",Multiple,outType);
            // set up the outputs
            outputs().resize(1);
            outputs()[0] = Connection("out",Multiple,outType);
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
            dlg.loader().addStringProperty("Global Property","Global",ns.data().getValue<std::string>(p,"GlobalName")); // field[2]
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
            ns.data().setValue(p,"GlobalName",v.GetString().ToStdString());
        }


        void load(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
        {
            NodeType::load(dlg,ns,p);
            dlg->addProp("Global Name",ns.data().getValue<std::string>(p,"GlobalName"));
        }

        void save(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
        {
            NodeType::save(dlg,ns,p);
            std::string v;
            dlg->get(NODEFLOW::PropField1,v);
            ns.data().setValue(p,"GlobalName",v);
        }


    };


    typedef GlobalValueNodeType<bool,Bool> GlobalBoolNodeType;
    typedef GlobalValueNodeType<int,Integer> GlobalIntegerNodeType;
    typedef GlobalValueNodeType<double,Float> GlobalFloatNodeType;
    typedef GlobalValueNodeType<std::string,String> GlobalStringNodeType;





}
#endif // VALUENODETYPE_H
