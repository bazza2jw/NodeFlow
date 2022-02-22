#include "valuenodetype.h"
#include "NodeFlow/nodeset.h"
#include "NodeFlow/PropertiesEditorDialog.h"
/*!
 * \brief The IntegerNodeType class
 */
class IntegerNodeType  : public  NODEFLOW::ValueNodeType<int,NODEFLOW::Integer>
{
public:
    IntegerNodeType(const std::string &s) : ValueNodeType<int,NODEFLOW::Integer>(s) {}
    virtual void load(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        ValueNodeType<int,NODEFLOW::Integer>::load(dlg,ns,p);
        dlg.loader().addIntProperty("Value","Value",ns.data().getValue<int>(p,"Value"));
    }
    virtual void save(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        ValueNodeType<int,NODEFLOW::Integer>::save(dlg,ns,p);
        wxVariant v = dlg.loader().fields()[NODEFLOW::PropField1]->GetValue();
        ns.data().setValue(p,"Value",v.GetInteger());
    }
};
/*!
 * \brief The DoubleNodeType class
 */
class DoubleNodeType  :   public  NODEFLOW::ValueNodeType<double,NODEFLOW::Float>
{
public:
    DoubleNodeType(const std::string &s) : NODEFLOW::ValueNodeType<double,NODEFLOW::Float>(s) {}
    virtual void load(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        ValueNodeType<double,NODEFLOW::Float>::load(dlg,ns,p);
        dlg.loader().addFloatProperty("Value","Value",ns.data().getValue<double>(p,"Value"));
    }
    virtual void save(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        ValueNodeType<double,NODEFLOW::Float>::save(dlg,ns,p);
        wxVariant v = dlg.loader().fields()[NODEFLOW::PropField1]->GetValue();
        ns.data().setValue(p,"Value",v.GetDouble());
    }
};
/*!
 * \brief The StringNodeType class
 */
class StringNodeType  :  public  NODEFLOW::ValueNodeType<std::string,NODEFLOW::String>
{
public:
    StringNodeType(const std::string &s) : NODEFLOW::ValueNodeType<std::string,NODEFLOW::String>(s) {}
    virtual void load(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        ValueNodeType<std::string,NODEFLOW::String>::load(dlg,ns,p);
        dlg.loader().addStringProperty("Value","Value",ns.data().getValue<std::string>(p,"Value"));
    }
    virtual void save(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        ValueNodeType<std::string,NODEFLOW::String>::save(dlg,ns,p);
        wxVariant v = dlg.loader().fields()[NODEFLOW::PropField1]->GetValue();
        ns.data().setValue(p,"Value",v.GetString().ToStdString());
    }
};
/*!
 * \brief The BoolNodeType class
 */
class BoolNodeType  :  public NODEFLOW::ValueNodeType<bool,NODEFLOW::Bool>
{
public:
    BoolNodeType(const std::string &s) : ValueNodeType<bool,NODEFLOW::Bool>(s) {}
    virtual void load(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        ValueNodeType<bool,NODEFLOW::Bool>::load(dlg,ns,p);
        dlg.loader().addBoolProperty("Value","Value",ns.data().getValue<bool>(p,"Value"));
    }
    virtual void save(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        ValueNodeType<bool,NODEFLOW::Bool>::save(dlg,ns,p);
        wxVariant v = dlg.loader().fields()[NODEFLOW::PropField1]->GetValue();
        ns.data().setValue(p,"Value",v.GetBool());
    }
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
    //
    NODEFLOW::NodeType::addType<NODEFLOW::GlobalBoolNodeType>("GlobalBool");
    NODEFLOW::NodeType::addType<NODEFLOW::GlobalIntegerNodeType>("GlobalInteger");
    NODEFLOW::NodeType::addType<NODEFLOW::GlobalFloatNodeType>("GlobalFloat");
    NODEFLOW::NodeType::addType<NODEFLOW::GlobalStringNodeType>("GlobalString");
    //
}
