#ifndef BUIAPP_H
#define BUIAPP_H
#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#   include "wx/wx.h"
#endif
#include <wx/cmdline.h>
#include "wx/xrc/xmlres.h"
#include <wx/docview.h>
#include <wx/image.h>
#include <wx/thread.h>
#include <Wt/WServer.h>
#include "NodeFlow/mqttconnection.h"



    class BuiApp : public wxApp
    {
        std::unique_ptr<Wt::WServer> _webThread; //! Web server thread
    public:
        BuiApp();
        
        virtual ~BuiApp(){}
        /*!
         * \brief OnInit
         * \return
         */
        bool OnInit();
        /*!
         * \brief OnInitCmdLine
         * \param parser
         */
        void OnInitCmdLine (wxCmdLineParser &parser);
        /*!
         * \brief OnCmdLineParsed
         * \param parser
         * \return true on success
         */
        bool OnCmdLineParsed (wxCmdLineParser &parser);
        /*!
         * \brief OnExit
         * \return
         */
        int OnExit();
    };

wxDECLARE_APP(BuiApp);
#endif // BUIAPP_H
