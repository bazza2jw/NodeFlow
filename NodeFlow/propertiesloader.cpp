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

#include "NodeFlow/propertiesloader.h"
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>

/*!
 * \brief setProperties
 * \param props
 */
void  PropertiesLoader::setProperties(wxArrayString &props)
{
    for(int i = 0; i < props.size(); i++)
    {
        wxStringTokenizer t(props[i], ","); // tokenise the string
        wxArrayString as;
        as.clear();
        while (t.HasMoreTokens()) {
            wxString token = t.GetNextToken();
            as.push_back(token);
        }
        setProperty(as);
    }
}

/*!
 * \brief PropertiesLoader::setProperty
 * \param props
 * \return
 */
wxPGProperty *  PropertiesLoader::setProperty(const wxString &props)
{
        wxStringTokenizer t(props, ","); // tokenise the string
        wxArrayString as;
        as.clear();
        while (t.HasMoreTokens()) {
            wxString token = t.GetNextToken();
            as.push_back(token);
        }
        wxPGProperty * p = setProperty(as);
        if(p)
        {
            _fields.push_back(p);
        }
        return p;
}


/*!
 * \brief PropertiesLoader::addBoolProperty
 * \param name
 * \param f
 */
wxBoolProperty * PropertiesLoader::addBoolProperty(const wxString &label, const wxString &name, bool f)
{
    wxBoolProperty *ret = new wxBoolProperty(label, name, f);
    _prop->Append(ret);
    _fields.push_back(ret);
    return ret;
}
/*!
 * \brief PropertiesLoader::addColourProperty
 * \param label
 * \param name
 * \param v
 * \return
 */
wxColourProperty * PropertiesLoader::addColourProperty(const wxString &label, const wxString &name, wxColour v)
{
    wxColourProperty *p = new wxColourProperty(label,name,v);
    _prop->Append(p);
    _fields.push_back(p);
    return p;
}


/*!
 * \brief PropertiesLoader::addIntProperty
 * \param name
 * \param value
 * \param min
 * \param max
 */
wxIntProperty * PropertiesLoader::addIntProperty(const wxString &label,const wxString &name, int value, int min, int max)
{
    wxIntProperty *p = new wxIntProperty(label, name, value);
    _prop->Append(p);
    p->SetAttribute(wxPG_ATTR_MAX,max);
    p->SetAttribute(wxPG_ATTR_MIN,min);
    p->SetEditor( wxT("SpinCtrl") );
    _fields.push_back(p);
    return p;
}
/*!
 * \brief PropertiesLoader::addFloatProperty
 * \param name
 * \param value
 * \param min
 * \param max
 */
wxFloatProperty * PropertiesLoader::addFloatProperty(const wxString &label,const wxString &name, double value, double min, double max)
{
    wxFloatProperty * p = new wxFloatProperty(label, name, value);
    _prop->Append(p);
    p->SetAttribute(wxPG_ATTR_MAX,max);
    p->SetAttribute(wxPG_ATTR_MIN,min);
    _fields.push_back(p);
    return p;
}
/*!
 * \brief PropertiesLoader::addStringProperty
 * \param name
 * \param value
 */
wxStringProperty * PropertiesLoader::addStringProperty(const wxString &label,const wxString &name, const wxString &value)
{
    wxStringProperty *p = new wxStringProperty(label, name, value);
    _prop->Append(p);
    _fields.push_back(p);
    return p;
}
/*!
 * \brief PropertiesLoader::addChoiceProperty
 * \param name
 * \param value
 */
wxEnumProperty * PropertiesLoader::addChoiceProperty(const wxString &label, const wxString &name, int value, const wxArrayString &labels, const wxArrayInt &values )
{
    wxEnumProperty *p = new wxEnumProperty(label,name,labels,values);
    _prop->Append(p);
    _fields.push_back(p);
    wxVariant v;
    p->IntToValue(v,value);
    p->SetValue(v);
    return p;
}

/*!
 * \brief PropertiesLoader::setProperty
 * \param as
 */
wxPGProperty *  PropertiesLoader::setProperty( const wxArrayString &as) {
    //
    // type,label,name,value,min,max
    //
    char c;
    as[0][0].GetAsChar(&c);
    switch (c) {
    case 'B': {
        // bool
        long v = 0;
        as[3].ToLong(&v);
        bool f = v ? true : false;
        return addBoolProperty(as[1], as[2], f);
    }
    break;
    case 'I': {
        // int
        long value = 0, min = 0, max =0;
        as[3].ToLong(&value);
        as[4].ToLong(&min);
        as[5].ToLong(&max);
        return addIntProperty(as[1],as[2],value,min,max);
    }
    break;
    case 'F': {
        //float
        double value,min,max;
        as[3].ToDouble(&value);
        as[4].ToDouble(&min);
        as[5].ToDouble(&max);
        return addFloatProperty(as[1],as[2],value,min,max);
    }
    break;
    case 'S': {
        return addStringProperty(as[1],as[2],as[3]);
    }
    break;
    case 'C':
        // choice
        break;
    default:
        // ignore everything else
        break;
    }
    return nullptr;
}




