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
#include "webaddobjectdialog.h"
#include "NodeFlow/nodetype.h"

/*!
    \brief MRL::WebAddObjectDialog::setup
    \param path parent object path
*/
void NODEFLOW::WebAddObjectForm::setup() {
    auto layout = contents()->setLayout(std::make_unique<Wt::WGridLayout>()); // a bag grid layout
    // add the items to the grid
    layout->addWidget(std::make_unique<Wt::WLabel>(_TR("Name")), 0, 0);
    _name = layout->addWidget(std::make_unique<Wt::WLineEdit>(), 0, 1);
    layout->addWidget(std::make_unique<Wt::WLabel>(_TR("Type")), 1, 0);
    _type = layout->addWidget(std::make_unique<Wt::WComboBox>(), 1, 1);
    // Set up the name validation handler
    for (auto i = NodeType::map().begin(); i != NodeType::map().end(); i++) {
        _type->addItem(i->first);
    }
    //
    WebDialogBase::setup();
}


void NODEFLOW::WebAddObjectForm::onOk() {
    //
    // dialog is modal but not does not block the add item function
    // It is recommended way to use Wt dialogs
    std::string n = name()->text().toUTF8();
    std::string t = type()->currentText().toUTF8();
    //
    // Add the node
    //
    unsigned id =  _set.addNode(t, _pt.x, _pt.y);
    NodePtr & np = _set.findNode(id);
    MRL::PropertyPath p;
    np->toPath(p);
    //
    _set.data().setValue(p,"Name",n);

}

