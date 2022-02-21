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
        wxVariant v = dlg.loader().fields()[2]->GetValue();
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
        wxVariant v = dlg.loader().fields()[2]->GetValue();
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
        wxVariant v = dlg.loader().fields()[2]->GetValue();
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
        dlg.loader().addStringProperty("Value","Value",ns.data().getValue<std::string>(p,"Value"));
    }
    virtual void save(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        ValueNodeType<bool,NODEFLOW::Bool>::save(dlg,ns,p);
        wxVariant v = dlg.loader().fields()[2]->GetValue();
        ns.data().setValue(p,"Value",v.GetBool());
    }
};
/*!
 * \brief The TimerNodeType class
 */
class TimerNodeType  :  public NODEFLOW::NodeType
{
    wxStopWatch _timer;
public:
    /*!
     * \brief TimerNodeType
     * \param s
     */
    TimerNodeType(const std::string &s) : NodeType(s)
    {
        _timer.Start();
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
                MRL::PropertyPath p;
                n->toPath(p);
                int interval = ns.data().getValue<int>(p,"Value");
                if(interval > 0) // get the value parameter
                {
                    if(_timer.TimeInMicro()/1000 > interval)
                    {
                        NODEFLOW::VALUE d;
                        setValueData("Timer", true, d);
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

    /*!
     * \brief properties
     * \param parent
     * \param ns
     * \param nodeId
     * \return
     */
    bool properties(wxWindow * parent,NODEFLOW::NodeSet &ns, unsigned nodeId)
    {
        MRL::PropertyPath p;
        NODEFLOW::NodePtr &n = ns.findNode(nodeId);
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
    virtual void load(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        dlg.loader().addStringProperty("Name","Name",ns.data().getValue<std::string>(p,"Name")); // field[0]
        dlg.loader().addBoolProperty("Enable Node","Enable",ns.data().getValue<bool>(p,"Enabled")); // field[1]
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
        wxVariant v = dlg.loader().fields()[0]->GetValue();
        ns.data().setValue(p,"Name",v.GetString().ToStdString());
        v = dlg.loader().fields()[1]->GetValue();
        ns.data().setValue(p,"Enabled",v.GetBool());
        v = dlg.loader().fields()[2]->GetValue();
        ns.data().setValue(p,"Value",v.GetInteger());
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
