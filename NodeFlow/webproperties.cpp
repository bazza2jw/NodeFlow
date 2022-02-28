#include "webproperties.h"
#include <Wt/WCssDecorationStyle.h>

/*!
 * \brief MRL::WebProperties::WebProperties
 * This is a generic table panel display - shows published values listed in the object's inputs
 */
NODEFLOW::WebProperties::WebProperties(NodeSet &set, NodePtr &n) :_set(set),_node(n)
{
    auto layout = std::make_unique<Wt::WFitLayout>();
    auto boxLayout = std::make_unique<Wt::WVBoxLayout>();
    _table = boxLayout->addWidget(std::make_unique<Wt::WTable>());
    //
    Wt::WLength tableHeight(10,Wt::LengthUnit::ViewportWidth );
    Wt::WLength tableWidth(50,Wt::LengthUnit::ViewportWidth );
    //
    Wt::WCssDecorationStyle s;
    Wt::WFont f;
    f.setSize(Wt::FontSize::XXLarge);
    s.setFont(f);
    //
    _table->setDecorationStyle(s);
    _table->setHeaderCount(1);
    _table->setWidth(Wt::WLength("100%"));
    _table->addStyleClass("table form-inline");
    //
    _table->toggleStyleClass("table-bordered", true);
    _table->toggleStyleClass("table-condensed", true);
    //
    _statusLine = boxLayout->addWidget(std::make_unique<Wt::WText>("Status"));
    layout->addItem(std::move(boxLayout));
    setLayout(std::move(layout));
    resize(tableWidth,tableHeight);
    //
}

/*!
 * \brief NODEFLOW::WebProperties::load
 */
void NODEFLOW::WebProperties::load()
{
}

/*!
 * \brief NODEFLOW::WebProperties::save
 */
void NODEFLOW::WebProperties::save()
{

}
/*!
 * \brief NODEFLOW::WebProperties::setProperty
 * \param as
 */
unsigned  NODEFLOW::WebProperties::setProperty( const std::string &as)
{
    MRL::StringVector l;
    MRL::toList(as,l,",");
    return setProperty(l);
}
/*!
 * \brief NODEFLOW::WebProperties::setProperty
 * \param as
 */
unsigned  NODEFLOW::WebProperties::setProperty( const MRL::StringVector &as) {
    //
    // type,label,name,value,min,max
    //
    char c = as[0][0];
    switch (c) {
    case 'B': {
        // bool
        int v = MRL::stringToNumber<int>(as[3]);
        bool f = v ? true : false;
        return addBoolProperty(as[1],f);
    }
    break;
    case 'I': {
        // int
        int value = 0, min = 0, max =0;
        value = MRL::stringToNumber<int>(as[3]);
        min = MRL::stringToNumber<int>(as[4]);
        max = MRL::stringToNumber<int>(as[5]);
        return addIntProperty(as[1],value,min,max);
    }
    break;
    case 'F': {
        //float
        double value,min,max;
        value = MRL::stringToNumber<double>(as[3]);
        min = MRL::stringToNumber<double>(as[4]);
        max = MRL::stringToNumber<double>(as[5]);
        return addFloatProperty(as[1],value,min,max);
    }
    break;
    case 'S': {
        return addStringProperty(as[1],as[3]);
    }
    break;
    case 'C':
    {
        // choice
        int value = MRL::stringToNumber<int>(as[3]);
        return addChoiceProperty(as[1],value,as[4]);
   }
        break;
    default:
        // ignore everything else
        break;
    }
    return 0;
}
/*!
 * \brief NODEFLOW::WebProperties::addStringProperty
 * \param label
 * \param v
 */
unsigned NODEFLOW::WebProperties::addStringProperty(const std::string &label, const std::string &v)
{
    _table->elementAt(_fieldId, 0)->addNew<Wt::WText>(label);
    _table->elementAt(_fieldId, 1)->addNew<Wt::WLineEdit>(v);
    return _fieldId++;
}
/*!
 * \brief NODEFLOW::WebProperties::addBoolProperty
 * \param label
 * \param v
 */
unsigned NODEFLOW::WebProperties::addBoolProperty(const std::string &label, bool v)
{
    _table->elementAt(_fieldId, 0)->addNew<Wt::WText>(label);
    Wt::WCheckBox *p = _table->elementAt(_fieldId, 1)->addNew<Wt::WCheckBox>();
    p->setChecked(v);
    return _fieldId++;
}
/*!
 * \brief NODEFLOW::WebProperties::addIntProperty
 * \param label
 * \param v
 * \param min
 * \param max
 */
unsigned NODEFLOW::WebProperties::addIntProperty(const std::string &label, int v, int min , int max)
{
    _table->elementAt(_fieldId, 0)->addNew<Wt::WText>(label);
    Wt::WSpinBox *p = _table->elementAt(_fieldId, 1)->addNew<Wt::WSpinBox>();
    p->setMinimum(min);
    p->setMaximum(max);
    p->setValue(v);
    return _fieldId++;
}
/*!
 * \brief NODEFLOW::WebProperties::addFloatProperty
 * \param label
 * \param v
 * \param min
 * \param max
 */
unsigned NODEFLOW::WebProperties::addFloatProperty(const std::string &label, double v, double min , double max )
{
    _table->elementAt(_fieldId, 0)->addNew<Wt::WText>(label);
    Wt::WDoubleSpinBox *p = _table->elementAt(_fieldId, 1)->addNew<Wt::WDoubleSpinBox>();
    p->setMinimum(min);
    p->setMaximum(max);
    p->setValue(v);
    return _fieldId++;
}
/*!
 * \brief NODEFLOW::WebProperties::addColourProperty
 * \param label
 * \param v
 */
unsigned NODEFLOW::WebProperties::addColourProperty(const std::string &label, const std::string &v)
{
    Wt::WColor c(v);
    _table->elementAt(_fieldId, 0)->addNew<Wt::WText>(label);
    _table->elementAt(_fieldId, 1)->addNew<Wt::WColorPicker>(c);
    return _fieldId++;
}

/*!
 * \brief NODEFLOW::WebProperties::addChoiceProperty
 * \param label
 * \param v
 * \param list
 */
unsigned NODEFLOW::WebProperties::addChoiceProperty(const std::string &label, int v, const std::string &list)
{
    MRL::StringVector l;
    MRL::PropertyPath pp;
    pp.toList(list); // parse the arguments
    _table->elementAt(_fieldId, 0)->addNew<Wt::WText>(label);
    Wt::WComboBox *p = _table->elementAt(_fieldId, 1)->addNew<Wt::WComboBox>();
    for(auto i = pp.begin(); i != pp.end(); i++) p->addItem(*i);
    p->setCurrentIndex(v);
    return _fieldId++;
}



