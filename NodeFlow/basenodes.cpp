#include "basenodes.h"
#include "nodetype.h"



static
/*!
 * \brief The LoadBaseNodes class
 */
class LoadBaseNodes
{
public:
    LoadBaseNodes()
    {
       NODEFLOW::NodeType::addType<NODEFLOW::BaseInput>("BaseInput");
       NODEFLOW::NodeType::addType<NODEFLOW::BaseOutput>("BaseOutput");
       NODEFLOW::NodeType::addType<NODEFLOW::BaseConnector>("BaseConnector");
    }
} _instance __attribute__ ((init_priority (2000)));
