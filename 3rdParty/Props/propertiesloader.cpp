#include "propertiesloader.h"
#include <wx/txtstrm.h>
#include <wx/wfstream.h>

void  PropertiesLoader::setProperty(const std::string &name, const std::string &s) {


    wxStringTokenizer t(s, "|"); // Pipe separated fields
    wxArrayString as;
    as.clear();
    while (t.HasMoreTokens()) {
        wxString token = t.GetNextToken();
        // process token here
        as.push_back(token);
    }
    //
    switch ((char)(s[0])) {
        case 'B': {
            // bool
            long v = 0;
            as[2].ToLong(&v);
            bool f = v ? true : false;
            _prop->Append(new wxBoolProperty(as[1], name, f));
        }
        break;
        case 'I': {
            // int
            long v = 0;
            as[2].ToLong(&v);
            auto p = _prop->Append(new wxIntProperty(as[1], name, v));
            as[4].ToLong(&v);
            p->SetAttribute(wxPG_ATTR_MAX,v);
            as[3].ToLong(&v);
            p->SetAttribute(wxPG_ATTR_MIN,v);
            p->SetEditor( wxT("SpinCtrl") );

        }
        break;
        case 'F': {
            //float
            double v = 0;
            as[2].ToDouble(&v);
            auto p = _prop->Append(new wxFloatProperty(as[1], name, v));
            as[4].ToDouble(&v);
            p->SetAttribute(wxPG_ATTR_MAX,v);
            as[3].ToDouble(&v);
            p->SetAttribute(wxPG_ATTR_MIN,v);
        }
        break;
        case 'S': {
            // string
            _prop->Append(new wxStringProperty(as[1], name, as[2]));
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

