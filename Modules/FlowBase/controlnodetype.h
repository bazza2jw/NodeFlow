#ifndef CONTROLNODETYPE_H
#define CONTROLNODETYPE_H
#include <MrlLib/mrllib.h>
#include "../../NodeFlow/nodetype.h"
#include "../../NodeFlow/nodeset.h"
#include "../../NodeFlow/fparser.hh"
#include "../../NodeFlow/PropertiesEditorDialog.h"


namespace NODEFLOW
{
    /*!
     * \brief The IfElseNodeType class
     * this is packet switching node based on an expresion compare
     */
    class IfElseNodeType : public NodeType
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

    public:
        enum
        {
            Input = 0, TrueOutput = 0, ElseOutput = 1
        };
        IfElseNodeType(const std::string &s = "IfElse") : NodeType(s),_vars(1){}
        virtual const char * nodeClass() const { return "Control";}
        virtual void setup();
        virtual void setupConnections();
        //
        bool compile(const std::string &func, const std::string &args = "a")
        {
            _vars.resize(1);
            return _parser.Parse(func,args) == -1;
        }
        //
        virtual void start(NodeSet &ns,  NodePtr &n)
        {
            MRL::PropertyPath p;
            n->toPath(p);
            std::string f =  ns.data().getValue<std::string>(p,"Function");
        }

        //
        bool process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data)
        {
            NodePtr &n = ns.findNode(nodeId);
            if(n && n->enabled())
            {
                switch(id)
                {
                case Input:
                {
                    _vars[0] = data[DATA_PAYLOAD].asDouble();
                    double r = _parser.Eval(_vars.data());
                    n->data()["OUT"] = std::floor(r);
                    if(r != 0)
                    {
                        // if true send the packet through the true output
                        return post(ns,nodeId,TrueOutput,data);
                    }
                    else {
                        // otherwise send out of the else output
                        return post(ns,nodeId,ElseOutput,data);
                    }
                }
                default:
                    break;
                }
            }
            return false;

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
            dlg.loader().addStringProperty("Function","Function",ns.data().getValue<std::string>(p,"Function")); // field[2]
        }

        virtual void save(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
        {
            wxVariant v = dlg.loader().fields()[0]->GetValue();
            ns.data().setValue(p,"Name",v.GetString().ToStdString());
            v = dlg.loader().fields()[1]->GetValue();
            ns.data().setValue(p,"Enabled",v.GetBool());
            v = dlg.loader().fields()[2]->GetValue();
            std::string f = v.GetString().ToStdString();
            ns.data().setValue(p,"Function",f);
        }


    };

    /*!
     * \brief The ElseIfNodeType class
     *  much the same as ifelse but the input is at the top easier for linking together with an if else , hence chaining
     */
    class ElseIfNodeType : public IfElseNodeType
    {
    public:
        ElseIfNodeType() : IfElseNodeType("ElseIf"){}
        virtual const char * nodeClass() const { return "Control";}
        virtual void setup();

    };


}
#endif // CONTROLNODETYPE_H
