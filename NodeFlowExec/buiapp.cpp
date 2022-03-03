#include "buiapp.h"
#include "../NodeFlow/NodeEditorMainframe.h"
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
    NODEFLOW::Plugin::initialiseAll(); // call the initialise function

//    static const char *av[] = {"OpcServant", "--docroot=.;/resources", "--http-address=0.0.0.0", "--http-port=8083","--resources-dir=./resources"};
//    //
//    std::string av0 = wxAppConsole::argv[0].ToStdString();
//    _webThread = std::make_unique<Wt::WServer>(av0);
//    _webThread->setServerConfiguration(5, (char **)(av), WTHTTP_CONFIGURATION);
//    _webThread->addEntryPoint(Wt::EntryPointType::Application,
//    [](const Wt::WEnvironment & env) {
//        auto app = std::make_unique<NODEFLOW::BuiWebApplication>(NODEFLOW::UI_ADMIN,env);
//        app->setTwoPhaseRenderingThreshold(0);
//        app->setTitle("Node Flow");
//        app->useStyleSheet("styles.css");
//        app->messageResourceBundle().use(Wt::WApplication::appRoot() + "about");
//        app->refresh();
//        return std::move(app);
//    },"/");

//    // initialise the model for the Wt model and link to the configuration and runtime
//    _webThread->start(); // start the server thread

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

