#include "nodeflowthread.h"

/*!
 * \brief Entry
 * \return exit code
 */
wxThread::ExitCode NODEFLOW::NodeFlowThread::Entry()
{
    try
    {
        wxStopWatch timer;
        NODEFLOW::VALUE v;
        _set.start();
        while( !GetThread()->TestDestroy() && !_stopThread)
        {
            timer.Start();
            _set.step(v);
            long dt = 20L - timer.Time(); // aim for a 20ms step
            if(dt > 0)
            {
                wxThread::Sleep(static_cast<unsigned>(dt));
            }
            else
            {
                wxThread::Sleep(1);
            }
        }
        _set.stop();
    }
    catch(std::exception &e)
    {
        EXCEPT_TRC;
    }
    catch (...) {
        EXCEPT_DEF;
    }
    wxLogDebug("Exit DAQ Thread");
    return wxThread::ExitCode(nullptr);
}

/*!
 * \brief OnKill
 */
void NODEFLOW::NodeFlowThread::OnKill()
{
    stop();
}

/*!
 * \brief start
 * Start the thread
 */
void NODEFLOW::NodeFlowThread::start()
{
    wxLogDebug("Start DAQ Thread");
    try
    {
        // Load the set
        _set.clear();
        if(wxFile::Exists(_flowName))
        {
            // proceed loading the file chosen by the user;
            if(_set.data().load(_flowName.ToStdString()))
            {
                _set.load();
                //
                _stopThread = false;
                CreateThread();
                GetThread()->SetPriority(wxPRIORITY_DEFAULT / 2); // this thread is not event driven
                GetThread()->Run();
            }
        }
    }
    CATCH_DEF

}

    /*!
     * \brief stop
     * Stop the thread
     */
    void NODEFLOW::NodeFlowThread::stop()
    {
        wxLogDebug("Stop NodeFlow Thread");
        _stopThread = true;
    }
