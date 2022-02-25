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
#ifndef BWEBNAVIGATIONTREE_H
#define BWEBNAVIGATIONTREE_H
#include "buiwebapplication.h"
#include <Wt/WTreeView.h>
#include <Wt/WTimer.h>
#include <Wt/WText.h>
#include <Wt/WPoint.h>
#include <Wt/WStandardItem.h>
#include <Wt/WStandardItemModel.h>
#include <Wt/WTreeView.h>

namespace NODEFLOW {
    class BWebNavigationTree;
    class WebDataModel : public Wt::WStandardItemModel {
        public:
            enum {
                // the columns
                Item = 0, Value,Type, Status,  NumberColumns
            };
        public:
            WebDataModel() :  Wt::WStandardItemModel(0, Value + 1) {
                setHeaderData(Item, Wt::Orientation::Horizontal, std::string("Item"));
                setHeaderData(Value, Wt::Orientation::Horizontal, std::string("Value"));
            }

    };
    typedef std::shared_ptr<WebDataModel> WebDataModelRef;

    /*!
        \brief The BWebNavigationTree class
    */
    class BWebNavigationTree : public Wt::WTreeView {
            Wt::WStandardItem *_currentItem = nullptr;
            Wt::WPoint _currentPosition;
            Wt::WTimer *_timer = nullptr;
            WebDataModelRef _model;
            //
        public:
            BWebNavigationTree();
            //

            void doMenu();

    protected:
            void selected(Wt::WModelIndex i, Wt::WMouseEvent e);
            void selectedDoubleClicked(Wt::WModelIndex i, Wt::WMouseEvent e);
            void onTimer();
    };
}
#endif // BWEBNAVIGATIONTREE_H
