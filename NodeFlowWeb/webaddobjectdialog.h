#ifndef WEBADDOBJECTDIALOG_H
#define WEBADDOBJECTDIALOG_H
#include "webdialog.h"
#include <Wt/WDialog.h>
#include <Wt/WComboBox.h>
#include <Wt/WLineEdit.h>
#include <Wt/WGridLayout.h>
#include <Wt/WToolBar.h>
#include "NodeFlow/nodeset.h"
namespace NODEFLOW {
    /*!
        \brief The WebAddObjectForm class
    */
    class WebAddObjectForm : public WebDialogBase {
            Wt::WComboBox *_type = nullptr;
            Wt::WLineEdit *_name = nullptr;
            wxPoint _pt; // hit point
            NODEFLOW::NodeSet & _set; // node set
        public:
            WebAddObjectForm(wxPoint &pt, NODEFLOW::NodeSet &s) : _pt(pt), _set(s)
             {

            }
            void setup();
            Wt::WComboBox *type() const {
                return  _type;
            }
            Wt::WLineEdit *name() const {
                return  _name;
            }
            void onOk();
    };
    /*!
     * \brief createColorButton
     * \param toolBar
     * \param className
     * \param text
     * \return push button - this has been added to the toolbar
     */
    inline Wt::WPushButton * createColorButton(Wt::WToolBar * toolBar, const char *className, const std::string & text)
    {
        auto button = Wt::cpp14::make_unique<Wt::WPushButton>();
        button->setTextFormat(Wt::TextFormat::XHTML);
        button->setText(text);
        button->addStyleClass(className);
        //
        Wt::WPushButton * b = button.get();
        toolBar->addButton(std::move(button));
        return b;
    }


}
#endif // WEBADDOBJECTDIALOG_H
