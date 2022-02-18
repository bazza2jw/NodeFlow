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




template <typename T, typename R>
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
    wxString _props;


public:

    BinaryTypeNode(const std::string s, BINARY_OPFUNC f, const wxString props = "Float,DefaultB,DefaultB,0.0,-10000000.0,10000000.0") : NodeType(s),_op(f),_props(props)
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
        inputs()[0] = Connection("A",Single);
        inputs()[1] = Connection("B",Single);
        //
        // set up the outputs
        outputs().resize(1);
        outputs()[0] = Connection("out",Multiple);
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
        auto w = dlg.loader().setProperty(_props);
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
        ns.data().setValue(p,"Enable DefaultB",v.GetBool());
    }


};

//

template <typename T, typename R>
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
        inputs()[0] = Connection("in",Single);
        //
        // set up the outputs
        outputs().resize(1);
        outputs()[0] = Connection("out",Multiple);
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

    static bool load(const std::string file);

    FunctionParser & parser() {
        return  _parser;
    }

    virtual void setup()
    {
        // get the function
        // get argument list
        // get number of arguments
        // parse it
        NodeType::setup();
    }

    virtual void setupConnections()
    {
        // get the number of inputs
        // create the input list
        // get number of outputs
        // create the output list
    }


//    bool evaluate(NodeSet &ns, unsigned nodeId, unsigned id,  VALUE &data );
//    virtual bool process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data);

};




}
#endif // FUNCTIONTYPENODE_H
