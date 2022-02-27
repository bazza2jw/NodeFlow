#ifndef DATAPROCESSINGTYPE_H
#define DATAPROCESSINGTYPE_H
#include <functional>
#include "../../NodeFlow/nodetype.h"
#include "../../NodeFlow/nodeset.h"
#include "../../NodeFlow/fparser.hh"
#include "../../NodeFlow/PropertiesEditorDialog.h"
#include <MrlLib/rollingbuffer.hpp>


namespace NODEFLOW
{

    // set an attritbute to a JSON value
    class AttributeSetterNodeType  : public NodeType
    {
        enum
        {
            Input = 0,
            Output = 0,
        };
    public:
        AttributeSetterNodeType(const std::string &s) : NodeType(s) {}
        virtual const char * nodeClass() const { return "Data Processor";}

        virtual bool process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data)
        {
            NodePtr &n = ns.findNode(nodeId);
            if(n && n->enabled())
            {

                switch(id)
                {
                case 0:
                {
                    MRL::PropertyPath p;
                    n->toPath(p);
                    VALUE result = data;
                    std::string an = ns.data().getValue<std::string>(p,"AttributeName");
                    result[an] = n->data(); // set the data
                    post(ns,nodeId,Output,result);
                }
                    break;
                default:
                    break;
                }
            }
            return false;
        }

        virtual void start(NodeSet &ns,  NodePtr &node)
        {
            try
            {
                if(node)
                {
                    MRL::PropertyPath p;
                    node->toPath(p);
                    std::string av = ns.data().getValue<std::string>(p,"AttributeValue");
                    MRL::stringToJson(av,node->data());
                }
            }
            CATCH_DEF
        }


        /*!
         * \brief setupConnections
         */
        void setupConnections()
        {
            inputs().resize(1);
            //
            inputs()[0] = Connection("in",Single,Any);
            //
            // set up the outputs
            outputs().resize(1);
            outputs()[0] = Connection("out",Multiple,Any); // pass through
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
            dlg.loader().addStringProperty("Attribute Name","AttributeName",ns.data().getValue<std::string>(p,"AttributeName"));
            dlg.loader().addStringProperty("Attribute Value","AttributeValue",ns.data().getValue<std::string>(p,"AttributeValue")); // the value is a JSON string
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
            wxVariant fa = dlg.loader().fields()[PropField1]->GetValue();
            ns.data().setValue(p,"AttributeName",fa.GetString().ToStdString());
            wxVariant fv = dlg.loader().fields()[PropField2]->GetValue();
            ns.data().setValue(p,"AttributeValue",fv.GetString().ToStdString());
        }


    };






    class StatisticsNodeType : public NodeType
    {

        enum
        {
            Input = 0,
            Output = 0,
            Mean,
            SD,
            Min,
            Max,
            Aggregated
        };
        class StatsNode : public Node
        {
            MRL::StatisticsBuffer _stats;
        public:
            StatsNode(unsigned id, unsigned type) : Node(id,type) {}
            MRL::StatisticsBuffer &stats() { return _stats;}
        };

        virtual NODEFLOW::Node * create(unsigned i)
        {
            return new StatsNode(i,id());
        }

    public:
        StatisticsNodeType(const std::string s) : NodeType(s)
        {

        }
        virtual const char * nodeClass() const { return "Data Processor";}

        virtual void start(NodeSet &ns,  NodePtr &node)
        {
            try
            {
                if(node)
                {
                    MRL::PropertyPath p;
                    node->toPath(p);
                    int w = ns.data().getValue<int>(p,"Width");
                    StatsNode *n = static_cast<StatsNode *>(node.get());
                    n->stats().clear();
                    n->stats().setWidth((w > 2)?w:5);
                }
            }
            CATCH_DEF
        }

        /*!
         * \brief process
         * \param ns
         * \param nodeId
         * \param id
         * \param data
         * \return
         */
        virtual bool process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data)
        {
            NodePtr &n = ns.findNode(nodeId);
            if(n && n->enabled())
            {
                switch(id)
                {
                case 0:
                {
                    StatsNode *sn = static_cast<StatsNode *>(n.get());
                    double v = data[DATA_PAYLOAD].asDouble();
                    sn->stats().addValue(v);
                    post(ns,nodeId,Output,data); // post on unchanged
                    if(sn->stats().size() > 2) // need more than two values to get a mean and sd
                    {
                        sn->stats().evaluate();
                        VALUE result;
                        //

                        double a = sn->stats().statistics().getMean();
                        setValueData(data,a,result);
                        post(ns,nodeId,Mean,result);
                        //
                        a = sn->stats().statistics().getStdDev();
                        setValueData(data,a,result);
                        post(ns,nodeId,SD,result);
                        //
                        a = sn->stats().statistics().getMinimum();
                        setValueData(data,a,result);
                        post(ns,nodeId,Min,result);
                        //
                        a = sn->stats().statistics().getMaximum();
                        setValueData(data,a,result);
                        post(ns,nodeId,Max,result);

                        result = data;
                        Json::Value v;
                        sn->stats().statistics().toJson(v);
                        result[DATA_PAYLOAD] = v;
                        post(ns,nodeId,Aggregated,result);

                    }
                }
                default:
                    break;
                }
            }
            return false;
        }


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
            outputs().resize(6);
            outputs()[0] = Connection("out",Multiple,Float); // pass through
            outputs()[1] = Connection("Mean",Multiple,Float);
            outputs()[2] = Connection("SD",Multiple,Float);
            outputs()[3] = Connection("Min",Multiple,Float);
            outputs()[4] = Connection("Max",Multiple,Float);
            outputs()[5] = Connection("Aggregated",Multiple,Any); // stats packet
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
            dlg.loader().addIntProperty("Buffer Width","Width",ns.data().getValue<int>(p,"Width"),5,2000); // field[2]
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
            wxVariant fv = dlg.loader().fields()[PropField1]->GetValue();
            ns.data().setValue(p,"Width",int(fv.GetInteger()));
        }



    };


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
                    setValueData(data,v,result);
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
         * \brief load
         * \param dlg
         * \param ns
         * \param p
         */
        virtual void load(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
        {
            NodeType::load(dlg,ns,p);
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
            NodeType::save(dlg,ns,p);
            wxVariant v = dlg.loader().fields()[PropField1]->GetValue();
            ns.data().setValue(p,"Scale",v.GetDouble());
            v = dlg.loader().fields()[PropField2]->GetValue();
            ns.data().setValue(p,"Offset",v.GetDouble());
        }
    };


    /*!
     * \brief The ThresholdTypeNode class
     * Hi Lo
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
                        setValueData(data,true,result);
                        post(ns,nodeId,HiOutput,result);
                        state = 3;

                    }
                    else if(v < n->data()["LOLIMIT"].asDouble())
                    {
                        setValueData(data,true,result);
                        post(ns,nodeId,LoOutput,result);
                        state = 2;

                    }
                    else
                    {
                        setValueData(data,true,result);
                        post(ns,nodeId,OkOutput,result);
                        state = 1;
                    }
                    setValueData(data,state,result);
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
         * \brief load
         * \param dlg
         * \param ns
         * \param p
         */
        virtual void load(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
        {
            NodeType::load(dlg,ns,p);
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
            NodeType::save(dlg,ns,p);
            wxVariant v = dlg.loader().fields()[PropField1]->GetValue();
            ns.data().setValue(p,"HiLimit",v.GetDouble());
            v = dlg.loader().fields()[PropField2]->GetValue();
            ns.data().setValue(p,"LoLimit",v.GetDouble());
        }
    };



    /*!
     * \brief The ThresholdTypeNode class
     * Hi Lo
     */
    class HiHiLoLoThresholdTypeNode : public NodeType // two inputs one output
    {
    public:
        enum
        {
            Input = 0,
            Output = 0,
            HiHiOutput,
            HiLoOutput,
            OkOutput,
            LoHiOutput,
            LoLoOutput,
            StateOutput

        };
    public:
        HiHiLoLoThresholdTypeNode(const std::string s) : NodeType(s)
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
            outputs().resize(7);
            outputs()[0] = Connection("Out",Multiple,Float);
            outputs()[1] = Connection("HiHi",Multiple,Bool);
            outputs()[2] = Connection("HiLo",Multiple,Bool);
            outputs()[3] = Connection("Ok",Multiple,Bool);
            outputs()[4] = Connection("LoHi",Multiple,Bool);
            outputs()[5] = Connection("LoLo",Multiple,Bool);
            outputs()[6] = Connection("State",Multiple,Integer);
        }
        /*!
         * \brief start
         * \param n
         */
        virtual void start(NodeSet &ns,  NodePtr &n)
        {
            n->data()["IN"] = 0.0;
            n->data()["HIHI"] = false;
            n->data()["HILO"] = false;
            n->data()["LOHI"] = false;
            n->data()["LOLO"] = false;
            n->data()["OK"] = false;
            //
            MRL::PropertyPath p;
            n->toPath(p);
            n->data()["HIHILIMIT"] = ns.data().getValue<double>(p,"HiHiLimit");
            n->data()["LOLOLIMIT"] = ns.data().getValue<double>(p,"LoLoLimit");
            n->data()["HILOLIMIT"] = ns.data().getValue<double>(p,"HiLoLimit");
            n->data()["LOHILIMIT"] = ns.data().getValue<double>(p,"LoHiLimit");
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
                    post(ns,nodeId,Output,data); // forward data

                    if(v > n->data()["HIHILIMIT"].asDouble())
                    {
                        setValueData(data,true,result);
                        post(ns,nodeId,HiHiOutput,result);
                        state = 5;

                    }
                    else if(v > n->data()["HILOLIMIT"].asDouble())
                    {
                        setValueData(data,true,result);
                        post(ns,nodeId,HiLoOutput,result);
                        state = 4;

                    }
                    else if(v < n->data()["LOLOLIMIT"].asDouble())
                    {
                        setValueData(data,true,result);
                        post(ns,nodeId,LoLoOutput,result);
                        state = 3;

                    }
                    else if(v < n->data()["LOHILIMIT"].asDouble())
                    {
                        setValueData(data,true,result);
                        post(ns,nodeId,LoHiOutput,result);
                        state = 2;

                    }
                    else
                    {
                        setValueData(data,true,result);
                        post(ns,nodeId,OkOutput,result);
                        state = 1;
                    }
                    setValueData(data,state,result);
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
         * \brief load
         * \param dlg
         * \param ns
         * \param p
         */
        virtual void load(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
        {
            NodeType::load(dlg,ns,p);
            dlg.loader().addFloatProperty("HiHi Limit","HiHiLimit",ns.data().getValue<double>(p,"HiHiLimit")); // field[2]
            dlg.loader().addFloatProperty("HiLo Limit","HiLoLimit",ns.data().getValue<double>(p,"HiLoLimit")); // field[3]
            dlg.loader().addFloatProperty("LoHi Limit","LoHiLimit",ns.data().getValue<double>(p,"LoHiLimit")); // field[2]
            dlg.loader().addFloatProperty("LoLo Limit","LoLoLimit",ns.data().getValue<double>(p,"LoLoLimit")); // field[3]
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
            NodeType::save(dlg,ns,p);
            wxVariant v;
            v = dlg.loader().fields()[PropField1]->GetValue();
            ns.data().setValue(p,"HiHiLimit",v.GetDouble());
            v = dlg.loader().fields()[PropField2]->GetValue();
            ns.data().setValue(p,"HiLoLimit",v.GetDouble());
            v = dlg.loader().fields()[PropField1]->GetValue();
            ns.data().setValue(p,"LoHiLimit",v.GetDouble());
            v = dlg.loader().fields()[PropField2]->GetValue();
            ns.data().setValue(p,"LoLoLimit",v.GetDouble());
        }
    };






}
#endif // DATAPROCESSINGTYPE_H
