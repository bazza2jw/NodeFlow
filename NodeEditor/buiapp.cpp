#include "buiapp.h"
#include "NodeEditorMainframe.h"
#include "../NodeFlow/nodeflowplugin.h"

BuiApp::BuiApp() : wxApp ()
{
    
}
/*!
 * \brief OnInit
 * \return
 */
bool BuiApp::OnInit()
{
    wxApp::OnInit(); // parse the command line
    wxInitAllImageHandlers();
    wxXmlResource::Get()->InitAllHandlers();
    NODEFLOW::Plugin::loadPlugins("/usr/local/NodeFlow/modules"); // load all plugins
    auto w = new NodeEditorMainframe(nullptr);
    w->Show();
    NODEFLOW::Plugin::initialiseAll(); // call the initialise function
    return true;
}

/*!
 * \brief OnInitCmdLine
 * \param parser
 */
void BuiApp::OnInitCmdLine (wxCmdLineParser &parser)
{
    wxApp::OnInitCmdLine(parser);
}

/*!
 * \brief OnCmdLineParsed
 * \param parser
 * \return
 */
bool BuiApp::OnCmdLineParsed (wxCmdLineParser &parser)
{
    wxApp::OnCmdLineParsed(parser);
    return true;
}

/*!
 * \brief OnExit
 * \return
 */
int  BuiApp::OnExit()
{
    NODEFLOW::Plugin::terminateAll(); // call the plugin terminate function
    return wxApp::OnExit();
}

