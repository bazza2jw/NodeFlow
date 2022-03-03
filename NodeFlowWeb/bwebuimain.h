/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of NodeFlow. NodeFlow C++ classes are free software: you can
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
#include "NodeFlow/node.h"

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
            WebCanvas * _canvas = nullptr;
            //
            VALUE _value;
        public:
            BWebUiMain();
            Wt::WNavigationBar *navigationBar() {
                return _navigationBar;
            }
protected:
            void onOpen();
            void onSave();
            void onSaveAs();
            void onStart();
            void onStep();
    };
}
#endif // BWEBUIMAIN_H
