#ifndef NODEFLOWTHREAD_H
#define NODEFLOWTHREAD_H

#include <MrlLib/mrllib.h>
#include "NodeFlow/nodeset.h"

namespace NODEFLOW
{
/*!
     * \brief The NodeFlowThread class
     * This is the flow thread - need to add some messaging to allow for stop/start/reload of node sets TBD
     */
    class NodeFlowThread  : public wxThreadHelper
    {
        bool _stopThread = false;
        NodeSet _set;
        wxString _flowName;
    public:
        NodeFlowThread() {}
        /*!
         * \brief Entry
         * \return exit code
         */
        wxThread::ExitCode Entry();
        /*!
         * \brief OnKill
         */
        void OnKill();
        /*!
         * \brief start
         * Start the thread
         */
        void start();
        /*!
         * \brief stop
         * Stop the thread
         */
        void stop();

         wxString & flowName() { return _flowName;}
    };
}
#endif // NODEFLOWTHREAD_H
