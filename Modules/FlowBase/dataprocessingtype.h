#ifndef DATAPROCESSINGTYPE_H
#define DATAPROCESSINGTYPE_H
#include <functional>
#include "../../NodeFlow/nodetype.h"
#include "../../NodeFlow/nodeset.h"
#include "../../NodeFlow/fparser.hh"
#include "../../NodeFlow/PropertiesEditorDialog.h"


namespace NODEFLOW
{


    class ScaleTypeNode : public NodeType // two inputs one output
    {
    public:
        enum
        {
            Input = 0,
            Output = 0
        };



    public:

        ScaleTypeNode(const std::string s) : NodeType(s)
        {

        }
        virtual const char * nodeClass() const { return "Data Processor";}
        /*!
         * \brief setupConnections
         */
        void setupConnections()
        {
            inputs().resize(1);
            //
            inputs()[0] = Connection("in",Single,Float);
            //
            // set up the outputs
            outputs().resize(1);
            outputs()[0] = Connection("out",Multiple,Float);
        }
        /*!
         * \brief start
         * \param n
         */
        virtual void start(NodeSet &ns,  NodePtr &n)
        {
            n->data()["IN"] = 0.0;
            n->data()["OUT"] = 0.0;
            //
            MRL::PropertyPath p;
            n->toPath(p);
            n->data()["SCALE"] = ns.data().getValue<double>(p,"Scale");
            n->data()["OFFSET"] = ns.data().getValue<double>(p,"Offset");
            //
        }

        /*!
         * \brief process
         * \param ns
         * \param nodeId
         * \param id
         * \param data
         * \return true on success
         */
        virtual bool process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data)
        {
            NodePtr &n = ns.findNode(nodeId);
            if(n && n->enabled())
            {
                switch(id)
                {
                case Input:
                {
                    double v = data[DATA_PAYLOAD].asDouble();
                    n->data()["IN"] = v;
                    v = n->data()["SCALE"].asDouble() * v +  n->data()["OFFSET"].asDouble();
                    VALUE result;
                    setValueData(data[DATA_TOPIC].asString(),v,result);
                    return post(ns,nodeId,Output,result);

                }
                    break;
                default:
                    break;
                }
            }
            return false;
        }

        /*!
         * \brief evaluate
         * \param ns
         * \param nodeId
         * \param id
         * \param data
         * \return
         */
        bool evaluate(NodeSet &ns, unsigned nodeId, unsigned id,  VALUE &data )
        {
            NodePtr  &n = ns.findNode(nodeId);
            if(n && n->enabled() && (id == Output))
            {
                VALUE a;
                if(calculate(ns,nodeId,Input,a))
                {
                    double r = n->data()["SCALE"].asDouble() * a[DATA_PAYLOAD].asDouble() +  n->data()["OFFSET"].asDouble();
                    data[DATA_PAYLOAD] = r;
                }
            }
            return false;
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
            dlg.loader().addFloatProperty("Scale","Scale",ns.data().getValue<double>(p,"Scale")); // field[2]
            dlg.loader().addFloatProperty("Offset","Offset",ns.data().getValue<double>(p,"Offset")); // field[3]
            //
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
            v = dlg.loader().fields()[2]->GetValue();
            ns.data().setValue(p,"Scale",v.GetDouble());
            v = dlg.loader().fields()[3]->GetValue();
            ns.data().setValue(p,"Offset",v.GetDouble());
        }
    };


    /*!
     * \brief The ThresholdTypeNode class
     * HiHi LoLo HiLo LoHi
     */
    class WindowThresholdTypeNode : public NodeType // two inputs one output
    {
    public:
        enum
        {
            Input = 0,
            HiOutput = 0,
            OkOutput = 1,
            LoOutput = 2,
            StateOutput = 3

        };
    public:
        WindowThresholdTypeNode(const std::string s) : NodeType(s)
        {

        }
        virtual const char * nodeClass() const { return "Data Processor";}
        /*!
         * \brief setupConnections
         */
        void setupConnections()
        {
            inputs().resize(1);
            inputs()[0] = Connection("in",Single,Float);
            // set up the outputs
            outputs().resize(4);
            outputs()[0] = Connection("Hi",Multiple,Bool);
            outputs()[1] = Connection("Ok",Multiple,Bool);
            outputs()[2] = Connection("Lo",Multiple,Bool);
            outputs()[3] = Connection("State",Multiple,Integer);
        }
        /*!
         * \brief start
         * \param n
         */
        virtual void start(NodeSet &ns,  NodePtr &n)
        {
            n->data()["IN"] = 0.0;
            n->data()["HI"] = false;
            n->data()["LO"] = false;
            n->data()["OK"] = false;
            //
            MRL::PropertyPath p;
            n->toPath(p);
            n->data()["HILIMIT"] = ns.data().getValue<double>(p,"HiLimit");
            n->data()["LOLIMIT"] = ns.data().getValue<double>(p,"LoLimit");
            //
        }

        /*!
         * \brief process
         * \param ns
         * \param nodeId
         * \param id
         * \param data
         * \return true on success
         */
        virtual bool process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data)
        {
            NodePtr &n = ns.findNode(nodeId);
            if(n && n->enabled())
            {
                VALUE result;
                switch(id)
                {
                case Input:
                {
                    double v = data[DATA_PAYLOAD].asDouble();
                    n->data()["IN"] = v;
                    int state = 0;
                    if(v > n->data()["HILIMIT"].asDouble())
                    {
                        setValueData(data[DATA_TOPIC].asString(),true,result);
                        post(ns,nodeId,HiOutput,result);
                        state = 3;

                    }
                    else if(v < n->data()["LOLIMIT"].asDouble())
                    {
                        setValueData(data[DATA_TOPIC].asString(),true,result);
                        post(ns,nodeId,LoOutput,result);
                        state = 2;

                    }
                    else
                    {
                        setValueData(data[DATA_TOPIC].asString(),true,result);
                        post(ns,nodeId,OkOutput,result);
                        state = 1;
                    }
                    setValueData(data[DATA_TOPIC].asString(),state,result);
                    post(ns,nodeId,StateOutput,result);

                }
                    break;
                default:
                    break;
                }
                return true;
            }
            return false;
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
            dlg.loader().addFloatProperty("Hi Limit","HiLimit",ns.data().getValue<double>(p,"HiLimit")); // field[2]
            dlg.loader().addFloatProperty("Lo Limit","LoLimit",ns.data().getValue<double>(p,"LoLimit")); // field[3]
            //
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
            v = dlg.loader().fields()[2]->GetValue();
            ns.data().setValue(p,"HiLimit",v.GetDouble());
            v = dlg.loader().fields()[3]->GetValue();
            ns.data().setValue(p,"LoLimit",v.GetDouble());
        }
    };






}
#endif // DATAPROCESSINGTYPE_H
