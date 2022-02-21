#include "flowbaseplugin.h"

void addFunctionNodes();
void addValueNodes();
void addControlNodes();
void addDataProcessing();
void addOutputs();

namespace NODEFLOW
{
/*!
     * \brief The FlowBasePlugin class
     */
    class FlowBasePlugin
    {
    public:
        FlowBasePlugin()
        {
            addFunctionNodes();
            addValueNodes();
            addControlNodes();
            addDataProcessing();
            addOutputs();
        }

    } _instance;
}
