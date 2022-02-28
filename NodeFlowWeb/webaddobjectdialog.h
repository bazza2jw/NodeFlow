#ifndef WEBADDOBJECTDIALOG_H
#define WEBADDOBJECTDIALOG_H

#include "NodeFlow/webdialog.h"
#include <Wt/WDialog.h>
#include <Wt/WComboBox.h>
#include <Wt/WLineEdit.h>
#include <Wt/WGridLayout.h>
#include <Wt/WToolBar.h>
#include "NodeFlow/nodeset.h"
#include "MrlLib/mrlsignals.h"

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
    //
    SIGNAL1(redrawRequest,int)
    //
};

class WebCanvas;
/*!
 * \brief The WebOpenFlowForm class
 */
class WebOpenFlowForm : public WebDialogBase {
    Wt::WComboBox *_files = nullptr;
    NODEFLOW::WebCanvas * _canvas; // node set
public:
    WebOpenFlowForm(NODEFLOW::WebCanvas *p) : _canvas(p)   {   }
    void setup();
    Wt::WComboBox *files() const { return  _files; }
    void onOk();
    //
};

/*!
 * \brief The WebSaveAsFlowForm class
 */
class WebSaveAsFlowForm : public WebDialogBase {
    Wt::WLineEdit *_name = nullptr;
    NODEFLOW::WebCanvas * _canvas; // node set
public:
    WebSaveAsFlowForm( NODEFLOW::WebCanvas *p) : _canvas(p)  { }
    void setup();
    Wt::WLineEdit *name() const { return  _name; }
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
