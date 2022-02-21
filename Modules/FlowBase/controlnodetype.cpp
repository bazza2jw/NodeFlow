#include "controlnodetype.h"
/*!
 * \brief addControlNodes
 */
void addControlNodes()
{
    NODEFLOW::NodeType::addType<NODEFLOW::ElseIfNodeType>("ElseIf");
    NODEFLOW::NodeType::addType<NODEFLOW::IfElseNodeType>("IfElse");
}
