#include "dataprocessingtype.h"

void addDataProcessing()
{
    NODEFLOW::NodeType::addType<NODEFLOW::ScaleTypeNode>("Scale");
    NODEFLOW::NodeType::addType<NODEFLOW::WindowThresholdTypeNode>("HiLoThreshold");
}
