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

#ifndef PROPERTIESLOADER_H
#define PROPERTIESLOADER_H
#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#   include "wx/wx.h"
#endif
#include <wx/propdlg.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>


/*!
 * \brief The PropertiesLoader class
 * load a set of properties into property grid based on a spec sheet
 */
class PropertiesLoader
{
    wxPropertyGridManager * _prop = nullptr;
    std::vector<wxPGProperty *> _fields; // set of property fields
public:
    PropertiesLoader(wxPropertyGridManager *prop) : _prop(prop){}
    void setProperties(wxArrayString &props);
    wxPGProperty * setProperty( const wxArrayString &spec);
    wxPGProperty * setProperty( const wxString &spec);

    wxBoolProperty *addBoolProperty(const wxString &label, const wxString &name, bool f = false);
    wxIntProperty *addIntProperty(const wxString &label,const wxString &name, int value, int min = -10000000, int max = 10000000);
    wxIntProperty *addUIntProperty(const wxString &label,const wxString &name, unsigned value,unsigned min = 0, unsigned max = 10000000)
    {
        return addIntProperty(label,name, int (value), int(min) , int(max));
    }

    wxFloatProperty * addFloatProperty(const wxString &label,const wxString &name, double value, double min = -1000000.0, double max = 1000000.0);
    wxStringProperty * addStringProperty(const wxString &label,const wxString &name, const wxString &value = "");
    wxEnumProperty * addChoiceProperty(const wxString &label, const wxString &name, int value, const wxArrayString &labels, const wxArrayInt &values = wxArrayInt());
    wxColourProperty * addColourProperty(const wxString &label, const wxString &name, wxColour v);
    std::vector<wxPGProperty *> & fields()  { return _fields;}

    template <typename T>  T get(unsigned i)
    {
        if(i < _fields.size())
        {
            wxAny v = _fields[i]->GetValue();
            return v.As<T>();

        }
        return T(0);
    }

    std::string get(unsigned i ) {
        if(i < _fields.size())
        {
            wxAny v = _fields[i]->GetValue();
            wxString s = v.As<wxString>();
            return s.ToStdString();
        }
    }
    //
    wxPGProperty * addProp( const wxString &label, const wxString &name, int v ) { return addIntProperty(label,name, v);}
    wxPGProperty * addProp( const wxString &label,const wxString &name, double v) { return addFloatProperty(label,name,v);}
    wxPGProperty * addProp( const wxString &label,const wxString &name, bool v ) { return addBoolProperty(label,name,v);}
    wxPGProperty * addProp( const wxString &label,const wxString &name, std::string & v ) { return addStringProperty(label,name,v);}
    wxPGProperty * addProp( const wxString &label,const wxString &name, wxColour v ) { return addColourProperty(label,name,v);}



};

#endif // PROPERTIESLOADER_H
