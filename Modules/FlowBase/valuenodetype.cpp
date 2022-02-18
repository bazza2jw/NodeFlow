#include "valuenodetype.h"
#include "NodeFlow/nodeset.h"

static class IntegerNodeType  : public  NODEFLOW::ValueNodeType<int>
{
public:
    IntegerNodeType() : ValueNodeType<int>("Integer") {}
    virtual void load(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        ValueNodeType<int>::load(dlg,ns,p);
        dlg.loader().addIntProperty("Value","Value",ns.data().getValue<int>(p,"Value"));
    }
    virtual void save(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        ValueNodeType<int>::save(dlg,ns,p);
        wxVariant v = dlg.loader().fields()[2]->GetValue();
        ns.data().setValue(p,"Value",v.GetInteger());
    }
} intInstance;

static class DoubleNodeType  :   public  NODEFLOW::ValueNodeType<double>
{
public:
    DoubleNodeType() : NODEFLOW::ValueNodeType<double>("Double") {}
    virtual void load(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        ValueNodeType<double>::load(dlg,ns,p);
        dlg.loader().addFloatProperty("Value","Value",ns.data().getValue<double>(p,"Value"));
    }
    virtual void save(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        ValueNodeType<double>::save(dlg,ns,p);
        wxVariant v = dlg.loader().fields()[2]->GetValue();
        ns.data().setValue(p,"Value",v.GetDouble());
    }
} doubleInstance;

class StringNodeType  :  public  NODEFLOW::ValueNodeType<std::string>
{
public:
    StringNodeType() : NODEFLOW::ValueNodeType<std::string>("String") {}
    virtual void load(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        ValueNodeType<std::string>::load(dlg,ns,p);
        dlg.loader().addStringProperty("Value","Value",ns.data().getValue<std::string>(p,"Value"));
    }
    virtual void save(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        ValueNodeType<std::string>::save(dlg,ns,p);
        wxVariant v = dlg.loader().fields()[2]->GetValue();
        ns.data().setValue(p,"Value",v.GetString().ToStdString());
    }
} stringInstance;

class BoolNodeType  :  public NODEFLOW::ValueNodeType<bool>
{
public:
    BoolNodeType() : ValueNodeType<bool>("Bool") {}
    virtual void load(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        ValueNodeType<bool>::load(dlg,ns,p);
        dlg.loader().addStringProperty("Value","Value",ns.data().getValue<std::string>(p,"Value"));
    }
    virtual void save(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        ValueNodeType<bool>::save(dlg,ns,p);
        wxVariant v = dlg.loader().fields()[2]->GetValue();
        ns.data().setValue(p,"Value",v.GetBool());
    }
} boolInstance;

static class TimerNodeType  :  public NODEFLOW::ValueNodeType<int>
{
    wxStopWatch _timer;
public:
    TimerNodeType() : ValueNodeType<int>("Timer")
    {
        _timer.Start();
    }

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
                        setValueData("Timer", 0, d);
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

    virtual void load(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        ValueNodeType<int>::load(dlg,ns,p);
        dlg.loader().addIntProperty("Interval (ms)","Value",ns.data().getValue<int>(p,"Value"));
    }
    virtual void save(PropertiesEditorDialog &dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
    {
        ValueNodeType<int>::save(dlg,ns,p);
        wxVariant v = dlg.loader().fields()[2]->GetValue();
        ns.data().setValue(p,"Value",v.GetInteger());
    }

} timerInstance;

