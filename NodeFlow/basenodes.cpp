#include "basenodes.h"
#include "nodetype.h"
#include "nodesettype.h"


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
       NODEFLOW::NodeType::addType<NODEFLOW::NodeSetType>("NodeSet");
       NODEFLOW::NodeType::addType<NODEFLOW::NodeSetOutputNodeType>("NodeSetOutput");
       NODEFLOW::NodeType::addType<NODEFLOW::NodeSetInputNodeType>("NodeSetInput");
    }
} _instance __attribute__ ((init_priority (2000)));
