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
#ifndef BUIWEBAPPLICATION_H
#define BUIWEBAPPLICATION_H

#include <Wt/WApplication.h>
#include <Wt/WEnvironment.h>
#include <Wt/WBootstrapTheme.h>
#include <Wt/Auth/AuthWidget.h>
#include <Wt/Auth/Login.h>

namespace Wt
{
    class WContainerWidget;
    class WNavigationBar;
    class WTreeTable;
    class WMenuItem;
    class WTreeView;
}
namespace NODEFLOW
{
    /*!
     * \brief The BuiWebApplication class
     */

    typedef enum
    {
      UI_NONE,
      UI_ADMIN,
      UI_USER,
      UI_REPORT,
      UI_DIAGNOSTIC
    } UI_TYPE;
/*!
     * \brief The BuiWebApplication class
     */
    class BuiWebApplication : public Wt::WApplication
    {
        //
        Wt::Auth::Login _login;
        Wt::WContainerWidget * _topContainer = nullptr;
        std::shared_ptr<Wt::WBootstrapTheme> _theme;
        UI_TYPE _type = UI_NONE;
    public:
        BuiWebApplication(UI_TYPE t, const Wt::WEnvironment& env);
        ~BuiWebApplication()
        {

        }

    };
}
#endif // BUIWEBAPPLICATION_H
