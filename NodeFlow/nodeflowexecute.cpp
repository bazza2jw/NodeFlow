#include "nodeflowexecute.h"

/*!
 * \brief NODEFLOW::NodeFlowExecute::NodeFlowExecute
 */
NODEFLOW::NodeFlowExecute::NodeFlowExecute()
{

}

bool NODEFLOW::NodeFlowExecute::load(const std::string &s)
{
    _set.clear();
    bool ret =  _set.data().load(s);
    _set.start(); // initalise the node set
    return ret;
}


void NODEFLOW::NodeFlowExecute::process()
{
    // read from the input queue and set the input value then call step. Call step even if no value is waiting.

}
