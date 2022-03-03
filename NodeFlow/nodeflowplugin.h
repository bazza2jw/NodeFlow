/*
 * Copyright (C) 2022 -  B. J. Hill
 *
 * This file is part of NodeFlow. NodeFlow C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */

#ifndef NODEFLOWPLUGIN_H
#define NODEFLOWPLUGIN_H
#include "nodetype.h"
#include <wx/log.h>
#include <wx/dir.h>
#include <wx/dynlib.h>
#include <wx/utils.h>
#include <wx/stdpaths.h>

namespace NODEFLOW
{

/*!
    \brief PLUGIN_PATH
*/
constexpr const char *PLUGIN_PATH = "Plugins"; // path in settings to plugins
/*!
    \brief PLUGIN_DIR
*/
constexpr const char *PLUGIN_DIR =  "modules"; // where plugins are found wrt root directory
/*!
    \brief The Plugin class
*/
class Plugin {
    wxDynamicLibrary _library;  //!< library object
    std::string _name; //!< plugin (short) name
    typedef std::unique_ptr<Plugin> PluginRef; //!< reference counted shared pointer
    typedef std::map<std::string,PluginRef> MAP;
    static MAP _map; // map of plugins
public:
    /*!
        \brief Plugin
        \param name
    */
    Plugin(const std::string &name);
    /*!
        \brief ~Plugin
    */
    virtual ~Plugin();
    /*!
        \brief loadPlugins
        \param dir
        \return true on success
    */
    static bool loadPlugins(const std::string &dir = PLUGIN_DIR);
    /*!
        \brief load
        \param dir
        \param f
        \return true on success
    */
    bool load(wxString &dir, wxString &f);


    /*!
        \brief library
        \return library handle
    */
    wxDynamicLibrary & library()  {
        return _library;
    }

    /*!
     * \brief find
     * \param n
     * \return
     */
    static Plugin * find(const std::string &n)
    {
        Plugin * ret = nullptr;
        auto i = _map.find(n);
        if(i != _map.end())
        {
            ret = (i->second).get();
        }
        return ret;
    }

    /*!
     * \brief clear
     */
    static void clear()
    {
        for(auto i = _map.begin(); i != _map.end(); i++)
        {
            PluginRef &r = (i->second);
            r.release();
        }
        _map.clear();
    }
    // Global initialise
    virtual void initialise() {}
    // Clean up
    virtual void terminate() {}
    //
    /*!
     * \brief callAllPlugins
     * \param func
     */
    static void callAllPlugins(std::function<void (Plugin *)> func);
    /*!
     * \brief initialiseAll
     */
    static void initialiseAll();
    /*!
     * \brief terminateAll
     * Full close down sequence
     */
    static void terminateAll();

};



}
#endif // NODEFLOWPLUGIN_H
