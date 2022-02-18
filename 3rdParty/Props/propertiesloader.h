#ifndef PROPERTIESLOADER_H
#define PROPERTIESLOADER_H



class PropertiesLoader
{
    wxPropertyGridManager * _prop = nullptr; //!< property grid
public:
    PropertiesLoader(wxPropertyGridManager *prop) : _prop(prop){}
    void setProperty(const std::string &name, const std::string &spec);
};

#endif // PROPERTIESLOADER_H
