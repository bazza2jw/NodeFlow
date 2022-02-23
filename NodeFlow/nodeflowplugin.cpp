#include "nodeflowplugin.h"
#include <MrlLib/mrllib.h>
/*
 * Copyright (C) 2022 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */


NODEFLOW::Plugin::MAP NODEFLOW::Plugin::_map; //!< map of plugins

/*!
 * \brief NODEFLOW::Plugin::Plugin
 * \param name
 */
NODEFLOW::Plugin::Plugin(const std::string &name)
{
    // register
    std::unique_ptr<Plugin> ptr(this); // add to map - self register
    _map.insert(std::make_pair(name,std::move(ptr)));
}

/*!
 * \brief NODEFLOW::Plugin::~Plugin
 */
NODEFLOW::Plugin::~Plugin()
{
    // should unload
}

/*!
 * \brief NODEFLOW::Plugin::load
 * \param dir
 * \param f
 * \return true on success
 */
bool NODEFLOW::Plugin::load(wxString &dir, wxString &f) // load all plugins in given directory
{
    // load plugins from directory
    wxString fpath = wxString::Format("%s/%s",dir,f);
    _library.Load(fpath);
    bool ret =  _library.IsLoaded();
    return ret;
}

/*!
 * \brief NODEFLOW::Plugin::loadPlugins
 * \param dir
 * \return true on success
 */
bool NODEFLOW::Plugin::loadPlugins(const std::string &dir)
{
    try
    {
        wxString dw(dir);
        wxDir d(dir);
        if(d.IsOpened())
        {
            wxString f;
#ifdef WIN32
            if(d.GetFirst (&f, "*.dll", wxDIR_FILES ))
#else
            if(d.GetFirst (&f, "*.so", wxDIR_FILES ))
#endif
            {
                do
                {
                    auto a = new Plugin(f.ToStdString());
                    if(a->load(dw,f)) // load the plugin
                    {
                        wxLogDebug("Loaded %s",f);
                    }
                    else
                    {
                        wxLogDebug("Failed To Load %s",f);
                    }
                } while(d.GetNext(&f));
            }
        }
        return true;
    }
    catch(...)
    {
        wxLogError("Exception loading libraries");
    }
    return false;
}


//
/*!
 * \brief callAllPlugins
 * \param func
 */
void NODEFLOW::Plugin::callAllPlugins(std::function<void (Plugin *)> func)
{
    for(auto i = _map.begin(); i != _map.end(); i++)
    {
        Plugin * p = (i->second).get();
        if(p)
        {
            func(p);
        }
    }
}

/*!
 * \brief initialiseAll
 */
void NODEFLOW::Plugin::initialiseAll()
{
    auto f = [](Plugin * p) {
        if (p) p->initialise();
    };
    callAllPlugins(f);
}
/*!
 * \brief terminateAll
 * Full close down sequence
 */
void NODEFLOW::Plugin::terminateAll()
{
    auto f = [](Plugin * p) {
        if (p)
        {
            p->terminate(); // ask plugins to terminate
            p->library().Unload(); // unload dll
        }
    };
    callAllPlugins(f); // terminate and unload
    _map.clear();
}

