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
#include "bwebuimain.h"
#include <Wt/WVBoxLayout.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WMenu.h>
#include <Wt/WMenuItem.h>
#include <Wt/WTabWidget.h>
#include <Wt/WTextArea.h>
#include <Wt/WLabel.h>
#include <Wt/WMessageBox.h>
#include "bwebnavigationtree.h"
#include <Wt/WFitLayout.h>
#include "webcanvas.h"
/*!
    \brief BWebUiMain::BWebUiMain
    \param parent
*/
NODEFLOW::BWebUiMain::BWebUiMain() {
    //
    auto layout = std::make_unique<Wt::WVBoxLayout>();
    //
    _navigationBar =  layout->addWidget(std::make_unique<Wt::WNavigationBar>());
    //
    _navigationBar->setTitle("Node Flow");
    auto fitlayout = layout->addLayout(std::make_unique<Wt::WFitLayout>(),5);
    //
    // Add menu
    auto m = std::make_unique<Wt::WMenu>();
    //
    // Add menu items and connect signals
    //
    _logout = m->addItem("Logout");
    _properties = m->addItem("System Properties");
    _alias = m->addItem("Alias Setup");
    _user = m->addItem("User Configuration");

    _navigationBar->addMenu(std::move(m));
    //
    // add the tab widget
    auto st = fitlayout->addWidget(std::make_unique<Wt::WTabWidget>());
    //
    // Navigator tab
    //
    std::unique_ptr<NODEFLOW::WebCanvas> ct((_canvas = new NODEFLOW::WebCanvas() ));
    //
    Wt::WLength wd(600,Wt::LengthUnit::Pixel);
    ct->resize(wd,wd);
    st->addTab(std::move(ct), "Node Editor");
    //
    // add a status bar
    //
    auto toolb = layout->addLayout(std::make_unique<Wt::WHBoxLayout>(), 1);
    _editItem = toolb->addWidget(std::make_unique<Wt::WPushButton>("Options..."), 1);
    _restartButton = toolb->addWidget(std::make_unique<Wt::WPushButton>("Restart"), 1);
    _startButton = toolb->addWidget(std::make_unique<Wt::WPushButton>("Start"), 1);
    _stopButton = toolb->addWidget(std::make_unique<Wt::WPushButton>("Stop"), 1);
    auto sb =   toolb->addWidget(std::make_unique<Wt::WLabel>(), 8);
    sb->setText("Status OK");
    //
    _logout->triggered().connect(this, &NODEFLOW::BWebUiMain::onLogout);
    _properties->triggered().connect(this, &NODEFLOW::BWebUiMain::onProperties);
    _alias->triggered().connect(this, &NODEFLOW::BWebUiMain::onAliasSetup);
    _editItem->clicked().connect(this, &NODEFLOW::BWebUiMain::onEditItem);
    _restartButton->clicked().connect(this, &NODEFLOW::BWebUiMain::onRestart);
    _startButton->clicked().connect(this, &NODEFLOW::BWebUiMain::onStart);
    _stopButton->clicked().connect(this, &NODEFLOW::BWebUiMain::onStop);
    _user->triggered().connect(this, &NODEFLOW::BWebUiMain::onUser);
    //
    setLayout(std::move(layout));
}

/*!
 * \brief NODEFLOW::BWebUiMain::onAliasSetup
 */
void NODEFLOW::BWebUiMain::onAliasSetup()
{
}

/*!
    \brief NODEFLOW::BWebUiMain::onRestart
*/
void NODEFLOW::BWebUiMain::onRestart() {
}
/*!
    \brief NODEFLOW::BWebUiMain::onStart
*/
void NODEFLOW::BWebUiMain::onStart() {
}
/*!
    \brief NODEFLOW::BWebUiMain::onStop
*/
void NODEFLOW::BWebUiMain::onStop() {
}



/*!
    \brief NODEFLOW::BWebUiMain::onLogout
*/
void NODEFLOW::BWebUiMain::onLogout() {
    Wt::StandardButton result = Wt::WMessageBox::show("Confirm", "Logout?",  Wt::StandardButton::Ok | Wt::StandardButton::Cancel);
    if (result == Wt::StandardButton::Ok) {
        // Do logout
        Wt::WApplication::instance()->quit();
    }
}

/*!
    \brief NODEFLOW::BWebUiMain::onProperties
*/
void NODEFLOW::BWebUiMain::onProperties() {
}

/*!
    \brief NODEFLOW::BWebUiMain::onEditItem
*/
void NODEFLOW::BWebUiMain::onEditItem() {
    _editItem->setFocus(false);
}

void NODEFLOW::BWebUiMain::onUser()
{
}
