#ifndef PROPERTIESVIEWERDIALOG_H
#define PROPERTIESVIEWERDIALOG_H
#include "PropertyPanels.h"
#include <MrlLib/mrllib.h>
#include <MrlLib/variantpropertytree.h>
/*!
 * \brief The PropertiesViewerDialog class
 */
class  PropertiesViewerDialog : public PropertiesViewerDialogBase
{
    MRL::VariantPropertyTree &_data;
    MRL::PropertyPath _path;
public:
    PropertiesViewerDialog(wxWindow *parent, MRL::VariantPropertyTree &data, MRL::PropertyPath &p) :
        PropertiesViewerDialogBase(parent),_data(data),_path(p)
    {

    }

    virtual ~PropertiesViewerDialog() {}
protected:
    virtual void OnClose(wxCommandEvent& event);
};
#endif // PROPERTIESVIEWERDIALOG_H
