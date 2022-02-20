#ifndef FUNCTIONTYPENODE_H
#define FUNCTIONTYPENODE_H
#include <functional>
#include "../../NodeFlow/nodetype.h"
#include "../../NodeFlow/nodeset.h"
#include "../../NodeFlow/fparser.hh"
#include "../../NodeFlow/PropertiesEditorDialog.h"

// function nodes are generated from the functions definitions file on startup
// functions are for double values

namespace NODEFLOW
{




template <typename T, typename R, ConnectionType inType = Any, ConnectionType outType = Any>
class BinaryTypeNode : public NodeType // two inputs one output
{
public:
    typedef std::function<R (T,T)> BINARY_OPFUNC;
    enum
    {
        InputA = 0,
        InputB,
        Output = 0
    };

    bool _haveA = false;
    bool _haveB = false;
    BINARY_OPFUNC _op;
    ConnectionType _inType = inType;


public:

    BinaryTypeNode(const std::string s, BINARY_OPFUNC f) : NodeType(s),_op(f)
    {

    }
    virtual const char * nodeClass() const { return "Math Operators";}
    /*!
     * \brief setupConnections
     */
    void setupConnections()
    {
        inputs().resize(2);
        //
        inputs()[0] = Connection("A",Single,inType);
        inputs()[1] = Connection("B",Single,inType);
        //
        // set up the outputs
        outputs().resize(1);
        outputs()[0] = Connection("out",Multiple,outType);
    }
    /*!
     * \brief start
     * \param n
     */
    virtual void start(NodeSet &ns,  NodePtr &n)
    {
        _haveA = false;
        n->data()["A"] = T(0);
        n->data()["OUT"] = T(0);
        MRL::PropertyPath p;
        n->toPath(p);
        _haveB = ns.data().getValue<bool>(p,"UseDefautB");
        n->data()["B"] =  ns.data().getValue<T>(p,"DefautB");;

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
            case InputA:
                n->data()["A"] = data[DATA_PAYLOAD].as<T>();
                _haveA = true;
                break;
            case InputB:
                n->data()["B"] = data[DATA_PAYLOAD].as<T>();
                _haveB = true;
                break;
            default:
                break;
            }
        }
        if(_haveA && _haveB)
        {
            R r = op(n->data()["A"].as<T>(), n->data()["B"].as<T>());
            VALUE result;
            setValueData(data[DATA_TOPIC].asString(),r,result);
            return post(ns,nodeId,Output,result);
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
            VALUE b;
            if(calculate(ns,nodeId,InputA,a) && calculate(ns,nodeId,InputB,b))
            {
                R r = op(a[DATA_PAYLOAD].as<T>(),b.as<T>()) ;
                data[DATA_PAYLOAD] = r;
            }
        }
        return false;
    }

    /*!
     * \brief op
     * \param a
     * \param b
     * \return a op b
     */
    R op(T a, T b) {
        try
        {
            return _op(a,b);
        }
        catch(...)
        {
            return 0;
        }
    }

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
        //
        char pb[128];
        sprintf(pb,getProps(inType),"DefaultB","DefaultB");
        //
        auto w = dlg.loader().setProperty(pb);
        T a = ns.data().getValue<T>(p,"DefaultB");
        wxVariant v(a);
        w->SetValue(v);
        dlg.loader().addBoolProperty("Use DefaultB","UseDefaultB",ns.data().getValue<bool>(p,"UseDefaultB")); // field[1]
    }

    virtual void save(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
    {
        wxVariant v = dlg.loader().fields()[0]->GetValue();
        ns.data().setValue(p,"Name",v.GetString().ToStdString());
        v = dlg.loader().fields()[1]->GetValue();
        ns.data().setValue(p,"Enabled",v.GetBool());
        wxAny a = (dlg.loader().fields()[2])->GetValue(); // get the default B field
        T val = a.As<T>();
        ns.data().setValue(p,"DefaultB",val);
        v = dlg.loader().fields()[3]->GetValue();
        ns.data().setValue(p,"UseDefaultB",v.GetBool());
    }
};



template <typename T,  ConnectionType inType = Any>
/*!
 * \brief The BinaryRelationalTypeNode class
 * Does a relational operation and routes the packet based on the result to the true or false outputs
 */
class BinaryRelationalTypeNode : public NodeType // two inputs one output
{
public:
    typedef std::function<bool (T,T)> BINARY_REL_OPFUNC;
    enum
    {
        InputA = 0,
        InputB,
        //
        TrueOutput = 0, // packet route if true
        Output = 1, // result
        FalseOutput = 2
    };

    bool _haveA = false;
    bool _haveB = false;
    BINARY_REL_OPFUNC _op;
    ConnectionType _inType = inType;


public:

    BinaryRelationalTypeNode(const std::string s, BINARY_REL_OPFUNC f) : NodeType(s),_op(f)
    {

    }
    virtual const char * nodeClass() const { return "Relation Operators";}
    /*!
     * \brief setupConnections
     */
    void setupConnections()
    {
        inputs().resize(2);
        //
        inputs()[0] = Connection("A",Single,inType);
        inputs()[1] = Connection("B",Single,inType);
        //
        // set up the outputs
        outputs().resize(3);
        outputs()[0] = Connection("true",Multiple,inType);
        outputs()[1] = Connection("out",Multiple,Bool);
        outputs()[2] = Connection("false",Multiple,inType);
    }
    /*!
     * \brief start
     * \param n
     */
    virtual void start(NodeSet &ns,  NodePtr &n)
    {
        _haveA = false;
        n->data()["A"] = T(0);
        n->data()["OUT"] = T(0);
        MRL::PropertyPath p;
        n->toPath(p);
        _haveB = ns.data().getValue<bool>(p,"UseDefautB");
        n->data()["B"] =  ns.data().getValue<T>(p,"DefautB");;

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
            case InputA:
                n->data()["A"] = data[DATA_PAYLOAD].as<T>();
                _haveA = true;
                break;
            case InputB:
                n->data()["B"] = data[DATA_PAYLOAD].as<T>();
                _haveB = true;
                break;
            default:
                break;
            }
        }
        if(_haveA && _haveB)
        {
            bool r = op(n->data()["A"].as<T>(), n->data()["B"].as<T>());
            VALUE result;
            setValueData(data[DATA_TOPIC].asString(),r,result);
            post(ns,nodeId,r?TrueOutput:FalseOutput,data); // route the packet
            return post(ns,nodeId,Output,result);
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
            VALUE b;
            if(calculate(ns,nodeId,InputA,a) && calculate(ns,nodeId,InputB,b))
            {
                R r = op(a[DATA_PAYLOAD].as<T>(),b.as<T>()) ;
                data[DATA_PAYLOAD] = r;
            }
        }
        return false;
    }

    /*!
     * \brief op
     * \param a
     * \param b
     * \return a op b
     */
    R op(T a, T b) {
        try
        {
            return _op(a,b);
        }
        catch(...)
        {
            return 0;
        }
    }

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
        //
        char pb[128];
        sprintf(pb,getProps(inType),"DefaultB","DefaultB");
        //
        auto w = dlg.loader().setProperty(pb);
        T a = ns.data().getValue<T>(p,"DefaultB");
        wxVariant v(a);
        w->SetValue(v);
        dlg.loader().addBoolProperty("Use DefaultB","UseDefaultB",ns.data().getValue<bool>(p,"UseDefaultB")); // field[1]
    }

    virtual void save(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
    {
        wxVariant v = dlg.loader().fields()[0]->GetValue();
        ns.data().setValue(p,"Name",v.GetString().ToStdString());
        v = dlg.loader().fields()[1]->GetValue();
        ns.data().setValue(p,"Enabled",v.GetBool());
        wxAny a = (dlg.loader().fields()[2])->GetValue(); // get the default B field
        T val = a.As<T>();
        ns.data().setValue(p,"DefaultB",val);
        v = dlg.loader().fields()[3]->GetValue();
        ns.data().setValue(p,"UseDefaultB",v.GetBool());
    }
};



//

template <typename T, typename R, ConnectionType inType = Any, ConnectionType outType = Any>
/*!
 * \brief The UnaryTypeNode class
 */
class UnaryTypeNode : public NodeType // two inputs one output
{
public:
    typedef std::function<R (T)>  UNARY_OPFUNC;
    enum
    {
        InputA = 0,
        Output = 0
    };

    UNARY_OPFUNC _op;


public:
    UnaryTypeNode(const std::string s, UNARY_OPFUNC f) : NodeType(s),_op(f)
    {

    }
    virtual const char * nodeClass() const { return "Unary Operators";}
    /*!
     * \brief setupConnections
     */
    void setupConnections()
    {
        inputs().resize(1);
        //
        inputs()[0] = Connection("in",Single,inType);
        //
        // set up the outputs
        outputs().resize(1);
        outputs()[0] = Connection("out",Multiple,outType);
    }
    /*!
     * \brief start
     * \param n
     */
    virtual void start(NodeSet &/*ns*/,  NodePtr &n)
    {
        n->data()["IN"] = T(0);
        n->data()["OUT"] = T(0);
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
            case InputA:
            {
                R r = op(data[DATA_PAYLOAD].as<T>());
                n->data()["OUT"] = r;
                VALUE result;
                setValueData(data[DATA_TOPIC].asString(),r,result);
                return post(ns,nodeId,Output,result);
            }
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
            if(calculate(ns,nodeId,InputA,a))
            {
                R r = op(a[DATA_PAYLOAD].as<T>()) ;
                data[DATA_PAYLOAD] = r;
                n->data()["OUT"] = r; // cache it
                return true;
            }
        }
        return false;
    }

    /*!
     * \brief op
     * \param a
     * \param b
     * \return a op b
     */
    T op(T a) {
        try
        {
            return _op(a);
        }
        catch(...)
        {
            return 0;
        }
    }
};

/*!
 * \brief The TopicChangeTypeNode class
 * Sets the topic for a packet
 */
class TopicChangeTypeNode : public NodeType // two inputs one output
{
public:
    enum
    {
        InputA = 0,
        Output = 0
    };



public:
    TopicChangeTypeNode(const std::string s) : NodeType(s)
    {

    }
    virtual const char * nodeClass() const { return "Topic Changers";}
    /*!
     * \brief setupConnections
     */
    void setupConnections()
    {
        inputs().resize(1);
        inputs()[0] = Connection("in",Multiple,Any);
        //
        // set up the outputs
        outputs().resize(1);
        outputs()[0] = Connection("out",Multiple,Any);
    }
    /*!
     * \brief start
     * \param n
     */
    virtual void start(NodeSet &ns,  NodePtr &n)
    {
        MRL::PropertyPath p;
        n->toPath(p);
        std::string t = ns.data().getValue<std::string>(p,"Topic");
        n->data()["TOPIC"] = t;
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
            case InputA:
            {
                VALUE result = data;
                result[DATA_TOPIC] = n->data()["TOPIC"]; // change the topic
                return post(ns,nodeId,Output,result);
            }
            default:
                break;
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
        dlg.loader().addStringProperty("Change Topic","Topic",ns.data().getValue<std::string>(p,"Topic")); // field[2]
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
        ns.data().setValue(p,"Topic",v.GetString().ToStdString());
    }


};

/*!
 * \brief The TopicFilterTypeNode class
 * Routes a packet based on the topic to either the match or reject outputs
 */
class TopicFilterTypeNode : public TopicChangeTypeNode // two inputs one output
{
public:
    enum
    {
        InputA = 0,
        MatchOutput = 0,
        RejectOutput = 1
    };

public:
    TopicFilterTypeNode(const std::string s) : TopicChangeTypeNode(s)
    {

    }
    virtual const char * nodeClass() const { return "Topic";}
    /*!
     * \brief setupConnections
     */
    void setupConnections()
    {
        inputs().resize(1);
        inputs()[0] = Connection("in",Multiple,Any);
        //
        // set up the outputs
        outputs().resize(2);
        outputs()[0] = Connection("match",Multiple,Any);
        outputs()[1] = Connection("reject",Multiple,Any);

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
            case InputA:
            {
                VALUE result = data;
                result[DATA_TOPIC] = n->data()["TOPIC"]; // change the topic
                return post(ns,nodeId,(data[DATA_TOPIC] == n->data()["TOPIC"])?MatchOutput:RejectOutput,result); // route the packet based on
            }
            default:
                break;
            }
        }
        return false;
    }
};



/*!
 * \brief The FunctionTypeNode class
 */
class FunctionTypeNode : public NodeType
{
 protected:
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
    MRL::VariantPropertyTree _config;

public:
    FunctionTypeNode(const std::string &s) : NodeType(s)
    {

    }
    /*!
     * \brief compile
     * \param func
     * \param args
     * \return
     */
    virtual bool compile(const std::string &func, const std::string &args = "a")
    {
        _vars.resize(1);
        return _parser.Parse(func,args) == -1;
    }

    FunctionParser & parser() {
        return  _parser;
    }

    /*!
     * \brief start
     */
    virtual void start(NodeSet &ns,  NodePtr &node)
    {
        MRL::PropertyPath p;
        node->toPath(p);
        std::string f = ns.data().getValue<std::string>(p,"Function");
        compile(f,"a"); // variable a is the input
    }


    virtual bool process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data)
    {
        NodePtr &n = ns.findNode(nodeId);
        if(n && n->enabled())
        {
            switch(id)
            {
            case 0:
            {
                double vars = data[DATA_PAYLOAD].asDouble();
                double  r = _parser.Eval(&vars);
                n->data()["OUT"] = r;
                VALUE result;
                setValueData(data[DATA_TOPIC].asString(),r,result);
                return post(ns,nodeId,Output,result);
            }
            default:
                break;
            }
        }
        return false;
    }



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


//    bool evaluate(NodeSet &ns, unsigned nodeId, unsigned id,  VALUE &data );
//    virtual bool process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data);

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
        dlg.loader().addStringProperty("Function","Function",ns.data().getValue<std::string>(p,"Function")); // field[2]
    }

    virtual void save(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
    {
        wxVariant v = dlg.loader().fields()[0]->GetValue();
        ns.data().setValue(p,"Name",v.GetString().ToStdString());
        v = dlg.loader().fields()[1]->GetValue();
        ns.data().setValue(p,"Enabled",v.GetBool());
        v = dlg.loader().fields()[2]->GetValue();
        ns.data().setValue(p,"Function",v.GetString().ToStdString());
    }


};




}
#endif // FUNCTIONTYPENODE_H
