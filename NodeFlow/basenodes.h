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
    void trigger(NodeSet &ns, NodePtr &node )
    {
        VALUE v(_value++); // generate an auto incrementing value
        post(ns,node->id(), BaseNodeType::BaseOutputId,v);
    }
    virtual Node * create(unsigned i)
    {
        Node * n = NodeType::create(i);
        n->setColour(*wxRED);
        return n;
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
    virtual Node * create(unsigned i)
    {
        Node * n = NodeType::create(i);
        n->setColour(*wxYELLOW);
        return n;
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
    virtual Node * create(unsigned i)
    {
        Node * n = NodeType::create(i);
        n->setColour(*wxGREEN);
        return n;
    }

};


}
#endif // BASENODES_H
