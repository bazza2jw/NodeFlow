#include "propertiesloader.h"
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
 * \brief PropertiesLoader::addBoolProperty
 * \param name
 * \param f
 */
void PropertiesLoader::addBoolProperty(const wxString &label, const wxString &name, bool f)
{
    _fields.push_back(_prop->Append(new wxBoolProperty(label, name, f)));
}
/*!
 * \brief PropertiesLoader::addIntProperty
 * \param name
 * \param value
 * \param min
 * \param max
 */
void PropertiesLoader::addIntProperty(const wxString &label,const wxString &name, int value, int min, int max)
{
    auto p = _prop->Append(new wxIntProperty(label, name, value));
    p->SetAttribute(wxPG_ATTR_MAX,max);
    p->SetAttribute(wxPG_ATTR_MIN,min);
    p->SetEditor( wxT("SpinCtrl") );
    _fields.push_back(p);
}
/*!
 * \brief PropertiesLoader::addFloatProperty
 * \param name
 * \param value
 * \param min
 * \param max
 */
void PropertiesLoader::addFloatProperty(const wxString &label,const wxString &name, double value, double min, double max)
{
    auto p = _prop->Append(new wxFloatProperty(label, name, value));
    p->SetAttribute(wxPG_ATTR_MAX,max);
    p->SetAttribute(wxPG_ATTR_MIN,min);
    _fields.push_back(p);
}
/*!
 * \brief PropertiesLoader::addStringProperty
 * \param name
 * \param value
 */
void PropertiesLoader::addStringProperty(const wxString &label,const wxString &name, const wxString &value)
{
    _fields.push_back(_prop->Append(new wxStringProperty(label, name, value)));
}
/*!
 * \brief PropertiesLoader::addChoiceProperty
 * \param name
 * \param value
 */
void PropertiesLoader::addChoiceProperty(const wxString &label, const wxString &name, int value)
{

}

/*!
 * \brief PropertiesLoader::setProperty
 * \param as
 */
void  PropertiesLoader::setProperty( const wxArrayString &as) {


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
            addBoolProperty(as[1], as[2], f);
        }
        break;
        case 'I': {
            // int
            long value = 0, min = 0, max =0;
            as[3].ToLong(&value);
            as[4].ToLong(&min);
            as[5].ToLong(&max);
            addIntProperty(as[1],as[2],value,min,max);
        }
        break;
        case 'F': {
            //float
            double value,min,max;
            as[3].ToDouble(&value);
            as[4].ToDouble(&min);
            as[5].ToDouble(&max);
            addFloatProperty(as[1],as[2],value,min,max);
        }
        break;
        case 'S': {
            addStringProperty(as[1],as[2],as[3]);
        }
        break;
        case 'C':
            // choice
            break;
        default:
            // ignore everything else
            break;
    }
}

