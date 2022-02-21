#ifndef BASENODES_H
#define BASENODES_H

#include "nodetype.h"
namespace NODEFLOW
{
/*!
 * \brief The BaseNodeType class
 */
class BaseNodeType : public NodeType
{
    size_t _in = 1;
    size_t _out = 1;
public:
    enum
    {
        BaseInputId = 0,
        BaseOutputId = 0
    };
    /*!
     * \brief BaseNodeType
     * \param n
     * \param nIn
     * \param nOut
     */
    BaseNodeType(const std::string &n, size_t nIn, size_t nOut)
        : NodeType(n),_in(nIn),_out(nOut) {}
    /*!
     * \brief setupConnections
     */
    void setupConnections()
    {
        // setup the inputs
        if(_in > 0)
        {
            inputs().resize(_in);
             inputs()[0] = Connection("in",Multiple);
        }
        // set up the outputs
        if(_out > 0)
        {
            outputs().resize(_out);
            outputs()[0] = Connection("out",Multiple);
        }
    }
};

/*!
 * \brief The BaseInput class
 */
class BaseInput : public BaseNodeType
{
    // input nodes only have one output
    unsigned _value = 0;
public:
    BaseInput(const std::string &n) : BaseNodeType(n,0,1) {}
     bool step(NodeSet &/*ns*/, NodePtr & /*node*/) { return true;}
    void trigger(NodeSet &ns, NodePtr &node )
    {
        VALUE v(_value++); // generate an auto incrementing value
        VALUE d;
         setValueData("BASEINPUT", v, d);
        post(ns,node->id(), BaseNodeType::BaseOutputId,d);
    }

};

/*!
 * \brief The BaseOutput class
 */
class BaseOutput : public BaseNodeType
{
public:
    BaseOutput(const std::string &n) : BaseNodeType(n,1,0) {}
    bool process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data)
    {
        // log the data
        std::cerr << "Node Id:"   << nodeId << " InputId:" << id << " Value:" << data << std::endl;
        return true;
    }
};

// log the packet and pass it through
/*!
 * \brief The BaseConnector class
 */
class BaseConnector : public BaseNodeType
{
public:
    BaseConnector(const std::string &n) : BaseNodeType(n,1,1) {}
    bool process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data)
    {
        // log the data
        std::cerr << "Node Id:"   << nodeId << " InputId:" << id << " Value:" << data << std::endl;
        post(ns,nodeId,BaseOutputId,data);
        return true;
    }

};


}
#endif // BASENODES_H
