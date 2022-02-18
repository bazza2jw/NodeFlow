#ifndef PROPERTIESVIEWERDIALOG_H
#define PROPERTIESVIEWERDIALOG_H
/*!
 * \brief The PropertiesViewerDialog class
 * View properties and notes read only
 */
class AMPITYPESSHARED_EXPORT PropertiesViewerDialog : public PropertiesViewerDialogBase,
    public PropertiesHelper {
        HDB::HdbPath _path;//!< path to database
    public:
        PropertiesViewerDialog(wxWindow *parent, const HDB::HdbPath &p) :
            PropertiesViewerDialogBase(parent),
            PropertiesHelper(m_properties),
            _path(p) {

        }

        virtual ~PropertiesViewerDialog() {}
        HDB::HdbPath &path() {
            return _path;
        }
        void setPath(const HDB::HdbPath &p) {
            _path = p;
            load(_path);
        }
        AMPI::PropertySheetType sheetType() {
            return AMPI::PropertySheetType::View;
        }

    protected:
    virtual void OnClose(wxCommandEvent& event);
        virtual void OnNotes(wxCommandEvent &/* event*/) {
            ViewNotesDialog dlg(this, _path);
            dlg.ShowModal();
        }
};
#endif // PROPERTIESVIEWERDIALOG_H
