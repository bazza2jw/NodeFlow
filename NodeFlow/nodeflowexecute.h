#ifndef NODEFLOWEXECUTE_H
#define NODEFLOWEXECUTE_H
//
#include "nodetype.h"
#include "nodeset.h"
#include "node.h"
//
namespace NODEFLOW
{
/*!
     * \brief The NodeFlowExecute class
     */
    class NodeFlowExecute
    {
        NodeSet _set;
        VALUEQUEUE _in;
    public:
        NodeFlowExecute();
        bool load(const std::string &s);
        void process();
        void post(const VALUE &v ) { _in.push(v);}
    };
}
#endif // NODEFLOWEXECUTE_H
