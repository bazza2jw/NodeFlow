#ifndef SCRIPTEDNODETYPE_H
#define SCRIPTEDNODETYPE_H

#include "NodeFlow/nodetype.h"
#include "NodeFlow/nodeset.h"
#include <wx/timer.h>
#include "NodeFlow/PropertiesEditorDialog.h"
#include "selene.h"


namespace NODEFLOW
{
    /*!
     * \brief The ScriptedNodeType class
     * LUA scripted nodes
     */
    class ScriptedNodeType : public NodeType
    {
        std::unique_ptr<sel::State> _state;
    public:
        ScriptedNodeType(const std::string &s) : NodeType(s) {}
        //
        virtual const char * nodeClass() const { return "Scripted";}
        virtual void start(NodeSet &ns,  NodePtr &n);
        void trigger(NodeSet &ns, NodePtr &n );
        void setupConnections();
        virtual bool properties(wxWindow * parent,NodeSet &ns, unsigned nodeId);
        virtual bool process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data);
        bool evaluate(NodeSet &ns, unsigned nodeId, unsigned id,  VALUE &data );
        virtual void setup();
        virtual bool setupApi(); // set up the API links so the LUA script can use C++ functions and C++ can call LUA functions
        //
        bool create();
        bool load(const std::string &file);
        void trace(std::string s) { wxLogMessage("Script:%s",s.c_str());}
        sel::State & state() { return *_state; }
        //
        //

    };
}
#endif // SCRIPTEDNODETYPE_H
