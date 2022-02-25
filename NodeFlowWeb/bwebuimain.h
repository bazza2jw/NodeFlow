/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#ifndef BWEBUIMAIN_H
#define BWEBUIMAIN_H
#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WGridLayout.h>
#include <Wt/WToolBar.h>
#include <Wt/WBorderLayout.h>
#include <Wt/WTree.h>
#include <Wt/WIconPair.h>
#include <Wt/WTreeNode.h>
#include <Wt/WNavigationBar.h>
#include <Wt/WBootstrapTheme.h>
#include "bwebnavigationtree.h"
#include <Wt/WPushButton.h>
namespace NODEFLOW {
    /*!
           \brief The BWebUiMain class
    */
    class WebCanvas;
    class BWebUiMain : public Wt::WContainerWidget {
            //
            Wt::WContainerWidget *_topContainer = nullptr;
            Wt::WNavigationBar *_navigationBar = nullptr;
            //
            // Top level menu items
            Wt::WMenuItem *_logout = nullptr;
            Wt::WMenuItem *_properties = nullptr;
            Wt::WMenuItem *_alias = nullptr;
            Wt::WMenuItem *_user = nullptr;


            Wt::WPushButton *_editItem = nullptr;
            Wt::WPushButton *_restartButton = nullptr;
            Wt::WPushButton *_startButton = nullptr;
            Wt::WPushButton *_stopButton = nullptr;
            WebCanvas * _canvas = nullptr;
            //
        public:
            BWebUiMain();
            Wt::WNavigationBar *navigationBar() {
                return _navigationBar;
            }
protected:
        void onLogout();
        void onProperties();
        void onEditItem();
        void onRestart();
        void onStart();
        void onStop();
        void onAliasSetup();
        void onUser();
    };
}
#endif // BWEBUIMAIN_H
