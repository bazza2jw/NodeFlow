#ifndef BUIAPP_H
#define BUIAPP_H

/*
 * Copyright (C) 2022 -  B. J. Hill
 *
 * This file is part of NodeFlow. NodeFlow C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
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
#include "NodeFlow/nodeset.h"
#include "nodeflowthread.h"

/*!
     * \brief The BuiApp class
     */

    class BuiApp : public wxAppConsole
    {
        std::unique_ptr<Wt::WServer> _webThread; //! Web server thread
        std::unique_ptr<NODEFLOW::NodeFlowThread> _flowThread;
        wxString _flowName;
    public:
        BuiApp();
        
        virtual ~BuiApp(){}
        /*!
         * \brief OnInit
         * \return
         */
        bool OnInit();

        int onRun();
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
