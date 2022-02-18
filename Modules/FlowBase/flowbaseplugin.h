#ifndef FLOWBASEPLUGIN_H
#define FLOWBASEPLUGIN_H
#include "../../NodeFlow/nodeflowplugin.h"

// basic nodes
// value source
// timer
// binary
// uniary
// function
// if / else
// debug output
// scale / offset
// threshold

namespace NODEFLOW
{
    class FlowBasePlugin : public Plugin
    {
    public:
        FlowBasePlugin() : Plugin("FlowBase") {}
    };
}
#endif // FLOWBASEPLUGIN_H
