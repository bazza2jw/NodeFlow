#ifndef OUTPUTNODETYPE_H
#define OUTPUTNODETYPE_H

#include <functional>
#include "../../NodeFlow/nodetype.h"
#include "../../NodeFlow/nodeset.h"
#include "../../NodeFlow/fparser.hh"
#include "../../NodeFlow/PropertiesEditorDialog.h"

namespace NODEFLOW
{
    // trace a message
    class TraceNodeType : public NodeType
    {
    public:
        /*!
         * \brief TraceNodeType
         * \param s
         */
        TraceNodeType(const std::string &s) : NodeType(s) {}

        /*!
         * \brief nodeClass
         * \return
         */
        virtual const char * nodeClass() const { return "Outputs";}
        /*!
         * \brief setupConnections
         */
        void setupConnections()
        {
            inputs().resize(1);
            inputs()[0] = Connection("in",Multiple,Any);
            // set up the outputs
            outputs().resize(1);
            outputs()[0] = Connection("out",Multiple,Any);
        }
        /*!
         * \brief process
         * \param ns
         * \param nodeId
         * \param id
         * \param data
         * \return
         */
        bool process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data)
        {
            // log the data
            std::cerr << "Node Id:"   << nodeId << " InputId:" << id << " Value:" << data << std::endl;
            return post(ns,nodeId,Output,data);
        }

    };



}
#endif // OUTPUTNODETYPE_H
