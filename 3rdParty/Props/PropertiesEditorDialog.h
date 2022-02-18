#ifndef PROPERTIESEDITORDIALOG_H
#define PROPERTIESEDITORDIALOG_H
/*!
 *
 *  Author:  B. J. Hill
 *  (c)  Air Monitors Limited 2017 -
 */
#include "ampitypes_global.h"
#include "TypesPanels.h"
#include "../AmpiCoreWindows/propertieshelper.h"
/*!
 * \brief The PropertiesEditorDialog class
 * Dialog to a database node's properties, notes and setting
 */
class AMPITYPESSHARED_EXPORT PropertiesEditorDialog : public PropertiesEditorDialogBase, public PropertiesHelper
{
    HDB::HdbPath _path;//!< path to object in database
public:
    PropertiesEditorDialog(wxWindow* parent, const HDB::HdbPath &p);
    virtual ~PropertiesEditorDialog();
    HDB::HdbPath &path() { return _path;}
    AMPI::PropertySheetType sheetType() { return AMPI::PropertySheetType::Edit;}
protected:
    virtual void OnNotes(wxCommandEvent& event);
    virtual void OnOk(wxCommandEvent& event);
    virtual void OnSettings(wxCommandEvent& event);
};
#endif // PROPERTIESEDITORDIALOG_H
