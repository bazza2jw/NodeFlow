#include "flowbaseplugin.h"

void addFunctionNodes();
void addValueNodes();
void addControlNodes();

namespace NODEFLOW
{
    class FlowBasePlugin
    {
    public:
        FlowBasePlugin()
        {
            addFunctionNodes();
            addValueNodes();
            addControlNodes();
        }

    } _instance;
}
