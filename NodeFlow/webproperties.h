#ifndef WEBPROPERTIES_H
#define WEBPROPERTIES_H
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WGridLayout.h>
#include <Wt/WToolBar.h>
#include <Wt/WBorderLayout.h>
#include <Wt/WTree.h>
#include <Wt/WIconPair.h>
#include <Wt/WTreeNode.h>
#include <Wt/WNavigationBar.h>
#include <Wt/WBootstrapTheme.h>
#include <Wt/WPushButton.h>
#include <Wt/WTabWidget.h>
#include <Wt/WFitLayout.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WTextArea.h>
#include <Wt/WTimer.h>
#include <Wt/WTable.h>
#include <Wt/WPanel.h>
#include <Wt/WSpinBox.h>
#include <Wt/WCheckBox.h>
#include <MrlLib/mrllib.h>
#include <Wt/WDoubleSpinBox.h>
#include <Wt/WColorPicker.h>
#include "webdialog.h"
#include "nodeset.h"
namespace NODEFLOW
{
    class WebProperties : public Wt::WContainerWidget
    {
        Wt::WTable * _table = nullptr;
        unsigned _fieldId = 0;
        Wt::WText * _statusLine = nullptr;
        NodeSet &_set;
        NodePtr &_node;
    public:

        typedef enum
        {
            String = 0,
            Int,
            Float,
            Bool,
            Colour,
            Choice
        } FieldType;

        WebProperties(NodeSet &s, NodePtr &n);

        Wt::WTable * table() { return _table; }
        //
        virtual void load();
        virtual void save();
        //

        // set of properties
        void  setProperties( const MRL::StringList &as)
        {
            for(auto i = as.begin(); i != as.end(); i++)
            {
                setProperty(*i);
            }
        }

        unsigned  setProperty( const std::string &as);

        unsigned  setProperty( const MRL::StringVector &as);

        unsigned addStringProperty(const std::string &label, const std::string &v);

        unsigned addBoolProperty(const std::string &label, bool v);

        unsigned addIntProperty(const std::string &label, int v, int min = -1000000, int max = 1000000);

        unsigned addFloatProperty(const std::string &label, double v, double min = -1000000, double max = 1000000);

        unsigned addColourProperty(const std::string &label, const std::string &v);

        unsigned addChoiceProperty(const std::string &label, int v, const std::string &list = "/Yes/No");
        //
        void get(unsigned i, bool &v ) { v =  getBool(i);}
        void get(unsigned i, int &v ) { v = getInt(i);}
        void get(unsigned i, double &v ) { v =  getFloat(i);}
        void get(unsigned i, std::string &v ) { v = getString(i);}
        //
        unsigned addProp(const std::string &label, int v ) { return addIntProperty(label,  v);}
        unsigned addProp(const std::string &label, double v) { return addFloatProperty(label,v);}
        unsigned addProp(const std::string &label, bool v ) { return addBoolProperty(label,v);}
        unsigned addProp(const std::string &label, const std::string & v ) { return addStringProperty(label,v);}
        //
        bool getBool(unsigned i)
        {
            Wt::WCheckBox *p = dynamic_cast<Wt::WCheckBox *>(_table->elementAt(i,1)->widget(0));
            return p->isChecked();
        }

        int getInt(unsigned i)
        {
            Wt::WSpinBox *p = dynamic_cast<Wt::WSpinBox *>(_table->elementAt(i,1)->widget(0));
            return p->value();
        }

        double getFloat(unsigned i)
        {
            Wt::WDoubleSpinBox *p = dynamic_cast<Wt::WDoubleSpinBox *>(_table->elementAt(i,1)->widget(0));
            return p->value();
        }

        std::string getString(unsigned i)
        {
            Wt::WLineEdit *p = dynamic_cast<Wt::WLineEdit *>(_table->elementAt(i,1)->widget(0));
            return p->text().toUTF8();
        }

        std::string getColour(unsigned i)
        {
            Wt::WColorPicker *p = dynamic_cast<Wt::WColorPicker *>(_table->elementAt(i,1)->widget(0));
            std::string s = p->color().name().toUTF8();
            return s;
        }

        int getChoice(unsigned i)
        {
            Wt::WComboBox *p = dynamic_cast<Wt::WComboBox *>(_table->elementAt(i,1)->widget(0));
            return p->currentIndex();
        }
    };

    /*!
     * \brief The WebPropertiesDialog class
     */
    class WebPropertiesDialog : public WebDialogBase
    {
        WebProperties *_props = nullptr;
        NodeSet & _set;
        NodePtr & _node;
    public:
        WebPropertiesDialog(NodeSet &s, NodePtr &n) : _set(s), _node(n) {}
        virtual void onOk()
        {
            MRL::PropertyPath p;
            _node->toPath(p);
            NodeType *t = _node->nodeType();
            if(t)
            {
                t->save(_props,_set,p);
            }
        }
        virtual void setup()
        {
            MRL::PropertyPath p;
            _node->toPath(p);
            auto layout = contents()->setLayout(std::make_unique<Wt::WFitLayout>());
            _props = layout->addWidget(std::make_unique<WebProperties>(_set,_node));
            NodeType *t = _node->nodeType();
            if(t)
            {
                t->load(_props,_set,p);
            }
            WebDialogBase::setup();
        }
    };
}
#endif // WEBPROPERTIES_H
