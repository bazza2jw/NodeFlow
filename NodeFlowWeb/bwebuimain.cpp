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
#include "webaddobjectdialog.h"
#include "webdialog.h"

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
    std::unique_ptr<Wt::WMenu> m = std::make_unique<Wt::WMenu>();
    std::unique_ptr<Wt::WMenu> filem = std::make_unique<Wt::WMenu>();
    std::unique_ptr<Wt::WMenu> actionm =  std::make_unique<Wt::WMenu>();
    //
    // File menu
    Wt::WMenuItem *i;
    i = filem->addItem("Open");
    i->triggered().connect(this,&NODEFLOW::BWebUiMain::onOpen);
    i = filem->addItem("Save");
    i->triggered().connect(this,&NODEFLOW::BWebUiMain::onSave);
    i = filem->addItem("Save As");
    i->triggered().connect(this,&NODEFLOW::BWebUiMain::onSaveAs);
    //
    // Action menu
    i = actionm->addItem("Start");
    i->triggered().connect(this,&NODEFLOW::BWebUiMain::onStart);
    i = actionm->addItem("Step");
    i->triggered().connect(this,&NODEFLOW::BWebUiMain::onStep);
    //
    m->addMenu("File",std::move(filem));
    m->addMenu("Action",std::move(actionm));
    //
    _navigationBar->addMenu(std::move(m));
    //
    // add the tab widget
    auto st = fitlayout->addWidget(std::make_unique<Wt::WTabWidget>());
    //
    // Navigator tab
    //
    auto wc = std::make_unique<Wt::WContainerWidget>();
    //
    auto canvasFitlayout = std::make_unique<Wt::WFitLayout>();
    _canvas = canvasFitlayout->addWidget(std::make_unique<NODEFLOW::WebCanvas>());

    wc->setLayout(std::move(canvasFitlayout));
    st->addTab(std::move(wc), "Node Editor",Wt::ContentLoading::Eager);

    Wt::WLength wd(900,Wt::LengthUnit::Pixel);
    st->resize(wd,wd);
    setLayout(std::move(layout));
}

/*!
 * \brief NODEFLOW::BWebUiMain::onOpen
 */
void NODEFLOW::BWebUiMain::onOpen()
{
    // open flow on server
    std::unique_ptr<NODEFLOW::WebOpenFlowForm> p = std::make_unique<NODEFLOW::WebOpenFlowForm>(_canvas);
    NODEFLOW::WebDialogBase::showDialog<>(this,p);

}
/*!
 * \brief NODEFLOW::BWebUiMain::onSave
 */
void NODEFLOW::BWebUiMain::onSave()
{
    // save flow to server
    _canvas->save();
}
/*!
 * \brief NODEFLOW::BWebUiMain::onSaveAs
 */
void NODEFLOW::BWebUiMain::onSaveAs()
{
    // save as
    std::unique_ptr<NODEFLOW::WebSaveAsFlowForm> p = std::make_unique<NODEFLOW::WebSaveAsFlowForm>(_canvas);
    NODEFLOW::WebDialogBase::showDialog<>(this,p);
}
/*!
 * \brief NODEFLOW::BWebUiMain::onStart
 */
void NODEFLOW::BWebUiMain::onStart()
{
    // Action a start
    _canvas->nodes().start();
}
/*!
 * \brief NODEFLOW::BWebUiMain::onStep
 */
void NODEFLOW::BWebUiMain::onStep()
{
    // drive a step
    _canvas->nodes().step(_value);
    // now read the output queue and print it
    NODEFLOW::NodeSet &st = _canvas->nodes();
    NODEFLOW::VALUEQUEUE &q = st.outValue();
    while(q.size() > 0)
    {
//        std::string s;
//        MRL::jsonToString(q.front(), s);
//        GetListTrace()->AppendString(s);
        q.pop();
    }
}



