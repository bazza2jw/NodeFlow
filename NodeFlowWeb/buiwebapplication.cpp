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
#include "buiwebapplication.h"
#include "bwebuimain.h"
#include <Wt/WEnvironment.h>
/*!
   * \brief NODEFLOW::BuiWebApplication::BuiWebApplication
   * \param env
   */
NODEFLOW::BuiWebApplication::BuiWebApplication(UI_TYPE t, const Wt::WEnvironment& env) : Wt::WApplication(env), _type(t)
{
    setTitle(" User Interface");
    // Get the boot strap theme for the navigation bar
    _theme = std::make_shared<Wt::WBootstrapTheme>();
    _theme->setVersion(Wt::BootstrapVersion::v3);
    setTheme(_theme);
    //
    _topContainer = root()->addWidget(std::make_unique<NODEFLOW::BWebUiMain>());
    //
    Wt::WLength wd(100,Wt::LengthUnit::Percentage);
    if(_topContainer) _topContainer->resize(wd,wd);
}

