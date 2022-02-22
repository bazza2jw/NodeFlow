#include "dataprocessingtype.h"

void addDataProcessing()
{
    NODEFLOW::NodeType::addType<NODEFLOW::ScaleTypeNode>("Scale"); // scaler and offset
    NODEFLOW::NodeType::addType<NODEFLOW::WindowThresholdTypeNode>("HiLoThreshold"); // high low range test
    NODEFLOW::NodeType::addType<NODEFLOW::StatisticsNodeType>("Statistics"); // rolling buffer stats
    NODEFLOW::NodeType::addType<NODEFLOW::HiHiLoLoThresholdTypeNode>("HiHiLoLo");// action / alert levels
}
