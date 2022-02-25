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
#include "bwebnavigationtree.h"
#include <Wt/WPopupMenu.h>
#include <Wt/WPopupMenuItem.h>

/*!
    \brief NODEFLOW::BWebNavigationTree::BWebNavigationTree
*/
NODEFLOW::BWebNavigationTree::BWebNavigationTree() : Wt::WTreeView(){

    setAttributeValue("oncontextmenu", "event.cancelBubble = true; event.returnValue = false; return false;");
    setSelectionMode(Wt::SelectionMode::Single); // single selection
    Wt::WFlags< Wt::EditTrigger > editTriggers;
    setEditTriggers(editTriggers);
    setRowHeight(34);
    _model = std::make_shared<NODEFLOW::WebDataModel>();
    setModel(_model);
    // do the connections
    mouseWentUp().connect(this, &NODEFLOW::BWebNavigationTree::selected);
    clicked().connect(this, &NODEFLOW::BWebNavigationTree::selected);
    doubleClicked().connect(this, &NODEFLOW::BWebNavigationTree::selectedDoubleClicked);
}

/*!
    \brief NODEFLOW::BWebNavigationTree::selectedDoubleClicked
    \param i
    \param e
*/
void NODEFLOW::BWebNavigationTree::selectedDoubleClicked(Wt::WModelIndex i, Wt::WMouseEvent e) {
    //_currentItem = Common::display().webStore()->itemFromIndex(i);
    if (_currentItem) {
        _currentPosition = Wt::WPoint(e.document().x,e.document().y);
        doMenu();
    }
}
/*!
    \brief NODEFLOW::BWebNavigationTree::doMenu
*/
void NODEFLOW::BWebNavigationTree::doMenu() {

    if (_currentItem) {

    }
}

/*!
    \brief NODEFLOW::BWebNavigationTree::selected
    \param i
    \param e
*/
void NODEFLOW::BWebNavigationTree::selected(Wt::WModelIndex i, Wt::WMouseEvent e) {
    // get the path to the item
    if(e.button() == Wt::MouseButton::Right)
    {
        selectedDoubleClicked(i,e);
    }
    else
    {
        //_currentItem = Common::display().webStore()->itemFromIndex(i);
        //
        if (_currentItem) {
            _currentPosition = Wt::WPoint(e.document().x,e.document().y);
        }
    }
}

/*!
    \brief NODEFLOW::BWebNavigationTree::OnTimer
*/
void NODEFLOW::BWebNavigationTree::onTimer() {
}


