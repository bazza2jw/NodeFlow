#ifndef VALUENODETYPE_H
#define VALUENODETYPE_H
#include "../../NodeFlow/nodetype.h"
#include "../../NodeFlow/nodeset.h"

#include <wx/timer.h>
#include "../../NodeFlow/PropertiesEditorDialog.h"

namespace NODEFLOW
{
//


// property edit cannot be templated elegantly
//
// value source
//
    template <typename T>
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

        /*!
         * \brief trigger
         * \param ns
         * \param n
         */
        void trigger(NodeSet &ns, NodePtr &n )
        {
            if(n && n->enabled())
            {
                MRL::PropertyPath p;
                n->toPath(p);
                T v = ns.data().getValue<T>("Value");
                VALUE d();
                post(ns,n->id(),0,d);
            }
        }

        void setupConnections()
        {
            inputs().resize(0);
            // set up the outputs
            outputs().resize(1);
            outputs()[0] = Connection("out",Multiple);
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




}
#endif // VALUENODETYPE_H
