#ifndef VALUENODETYPE_H
#define VALUENODETYPE_H
#include "NodeFlow/nodetype.h"
#include "NodeFlow/nodeset.h"
#include <wx/timer.h>
#include "NodeFlow/PropertiesEditorDialog.h"

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
                T v = ns.data().getValue<T>("Value");
                VALUE result;
                setValueData(data[DATA_TOPIC].asString(),r,result);
                return post(ns,nodeId,Output,result);
            }
        }

        void setupConnections()
        {
            inputs().resize(1);
            inputs()[0] =  Connection("trig",Multiple,Bool);
            // set up the outputs
            outputs().resize(1);
            outputs()[0] = Connection("out",Multiple,outType);
        }
        /*!
         * \brief properties
         * \param parent
         * \param ns
         * \param nodeId
         * \return
         */
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
        }

        virtual void save(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
        {
            wxVariant v = dlg.loader().fields()[0]->GetValue();
            ns.data().setValue(p,"Name",v.GetString().ToStdString());
            v = dlg.loader().fields()[1]->GetValue();
            ns.data().setValue(p,"Enabled",v.GetBool());
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
                ns.data().setValue(p,n->data()["GLOBALNAME"].asString(),v); // set the global value
                VALUE result = data;
                return post(ns,nodeId,Output,result);
            }
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
         * \brief properties
         * \param parent
         * \param ns
         * \param nodeId
         * \return
         */
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
        /*!
         * \brief load
         * \param dlg
         * \param ns
         * \param p
         */
        virtual void load(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
        {
            dlg.loader().addStringProperty("Name","Name",ns.data().getValue<std::string>(p,"Name")); // field[0]
            dlg.loader().addBoolProperty("Enable Node","Enable",ns.data().getValue<bool>(p,"Enabled")); // field[1]
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
            wxVariant v = dlg.loader().fields()[0]->GetValue();
            ns.data().setValue(p,"Name",v.GetString().ToStdString());
            v = dlg.loader().fields()[1]->GetValue();
            ns.data().setValue(p,"Enabled",v.GetBool());
            wxVariant v = dlg.loader().fields()[2]->GetValue();
            ns.data().setValue(p,"GlobalName",v.GetString().ToStdString());
        }

    };


    typedef GlobalValueNodeType<bool,Bool> GlobalBoolNodeType;
    typedef GlobalValueNodeType<int,Integer> GlobalIntegerNodeType;
    typedef GlobalValueNodeType<double,Float> GlobalFloatNodeType;
    typedef GlobalValueNodeType<std::string,String> GlobalStringNodeType;





}
#endif // VALUENODETYPE_H
