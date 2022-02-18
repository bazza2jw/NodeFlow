#ifndef PROPERTIESEDITORDIALOG_H
#define PROPERTIESEDITORDIALOG_H
#include "PropertyPanels.h"
#include <MrlLib/mrllib.h>
#include <MrlLib/variantpropertytree.h>
#include "propertiesloader.h"
/*!
 * \brief The PropertiesEditorDialog class
 */
class  PropertiesEditorDialog : public PropertiesEditorDialogBase
{
    PropertiesLoader _loader;
    MRL::VariantPropertyTree &_data;
    MRL::PropertyPath &_path;
public:
    PropertiesEditorDialog(wxWindow* parent, MRL::VariantPropertyTree &data,  MRL::PropertyPath &p);
    virtual ~PropertiesEditorDialog();
    PropertiesLoader & loader() { return _loader;}
 protected:

    virtual void OnNotes(wxCommandEvent& event);
    virtual void OnSettings(wxCommandEvent& event);
    virtual void OnOk(wxCommandEvent& event);

    virtual void load() {} // read from tree
    virtual void save() {} // write to tree

};
#endif // PROPERTIESEDITORDIALOG_H
