#ifndef PROPERTIESLOADER_H
#define PROPERTIESLOADER_H
#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#   include "wx/wx.h"
#endif
#include <wx/propdlg.h>
#include <wx/propgrid/propgrid.h>


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
    void setProperty( const wxArrayString &spec);
    void addBoolProperty(const wxString &label, const wxString &name, bool f);
    void addIntProperty(const wxString &label,const wxString &name, int value, int min, int max);
    void addFloatProperty(const wxString &label,const wxString &name, double value, double min, double max);
    void addStringProperty(const wxString &label,const wxString &name, const wxString &value);
    void addChoiceProperty(const wxString &label,const wxString &name, int value);
    std::vector<wxPGProperty *> & fields() const { return _fields;}
};

#endif // PROPERTIESLOADER_H
