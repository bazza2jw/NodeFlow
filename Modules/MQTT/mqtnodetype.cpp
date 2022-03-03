/*
 * Copyright (C) 2022 -  B. J. Hill
 *
 * This file is part of NodeFlow. NodeFlow C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#include "mqtnodetype.h"
#include "NodeFlow/propertiesloader.h"
#include "NodeFlow/webproperties.h"
#include "NodeFlow/PropertiesEditorDialog.h"


static class MQQTPlugin
{
public:
    MQQTPlugin()
    {
         NODEFLOW::NodeType::addType< NODEFLOW::MqttInputNodeType>("MQTT_Input");
         NODEFLOW::NodeType::addType< NODEFLOW::MqttOutputNodeType>("MQTT_Output");
    }

} _instance;
/*!
 * \brief load
 * \param dlg
 * \param ns
 * \param p
 */
void NODEFLOW::MqttInputNodeType::load(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
{
    NodeType::load(dlg,ns,p);
    dlg.loader().addStringProperty("Username","Username",ns.data().getValue<std::string>(p,"Username"));
    dlg.loader().addStringProperty("Password","Password",ns.data().getValue<std::string>(p,"Password"));
    dlg.loader().addStringProperty("Host","Host",ns.data().getValue<std::string>(p,"Host"));
    dlg.loader().addIntProperty("Port","Port",ns.data().getValue<int>(p,"Port"),1,64000);
    dlg.loader().addStringProperty("Subscribe","Subscribe",ns.data().getValue<std::string>(p,"Subscribe"));
    dlg.loader().addStringProperty("Item","Item",ns.data().getValue<std::string>(p,"Item"));
}
/*!
 * \brief save
 * \param dlg
 * \param ns
 * \param p
 */
void NODEFLOW::MqttInputNodeType::save(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
{
    NodeType::save(dlg,ns,p);
    ns.data().setValue(p,"Username",dlg.loader().get<wxString>(PropField1).ToStdString());
    ns.data().setValue(p,"Password",dlg.loader().get<wxString>(PropField2).ToStdString());
    ns.data().setValue(p,"Host",dlg.loader().get<wxString>(PropField3).ToStdString());
    ns.data().setValue(p,"Port",dlg.loader().get<int>(PropField4));
    ns.data().setValue(p,"Subscribe",dlg.loader().get<wxString>(PropField5).ToStdString());
    ns.data().setValue(p,"Item",dlg.loader().get<wxString>(PropField6).ToStdString());
}
/*!
 * \brief load
 * \param dlg
 * \param ns
 * \param p
 */
void NODEFLOW::MqttInputNodeType::load(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
{
    NodeType::load(dlg,ns,p);
    dlg->addStringProperty("Username",ns.data().getValue<std::string>(p,"Username"));
    dlg->addStringProperty("Password",ns.data().getValue<std::string>(p,"Password"));
    dlg->addStringProperty("Host",ns.data().getValue<std::string>(p,"Host"));
    dlg->addIntProperty("Port",ns.data().getValue<int>(p,"Port"),1,64000);
    dlg->addStringProperty("Subscribe",ns.data().getValue<std::string>(p,"Subscribe"));
    dlg->addStringProperty("Item",ns.data().getValue<std::string>(p,"Item"));
}
/*!
 * \brief save
 * \param dlg
 * \param ns
 * \param p
 */
void NODEFLOW::MqttInputNodeType::save(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
{
    NodeType::save(dlg,ns,p);
    std::string  v =  dlg->getString(NODEFLOW::PropField1);
    ns.data().setValue(p,"Username",v);
    v = dlg->getString(NODEFLOW::PropField2);
    ns.data().setValue(p,"Password",v);
    v = dlg->getString(NODEFLOW::PropField3);
    ns.data().setValue(p,"Host",v);
    ns.data().setValue(p,"Port",dlg->getInt(NODEFLOW::PropField4));
    v = dlg->getString(NODEFLOW::PropField5);
    ns.data().setValue(p,"Subscribe",v);
    v = dlg->getString(NODEFLOW::PropField6);
    ns.data().setValue(p,"Item",v);

}
// ********************************* OUTPUT MQTT **********************************************
/*!
 * \brief load
 * \param dlg
 * \param ns
 * \param p
 */
void NODEFLOW::MqttOutputNodeType::load(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
{
    NodeType::load(dlg,ns,p);
    dlg.loader().addStringProperty("Username","Username",ns.data().getValue<std::string>(p,"Username"));
    dlg.loader().addStringProperty("Password","Password",ns.data().getValue<std::string>(p,"Password"));
    dlg.loader().addStringProperty("Host","Host",ns.data().getValue<std::string>(p,"Host"));
    dlg.loader().addIntProperty("Port","Port",ns.data().getValue<int>(p,"Port"),1,64000);
    dlg.loader().addStringProperty("","Item",ns.data().getValue<std::string>(p,"Item"));
}
/*!
 * \brief save
 * \param dlg
 * \param ns
 * \param p
 */
void NODEFLOW::MqttOutputNodeType::save(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p)
{
    NodeType::save(dlg,ns,p);
    ns.data().setValue(p,"Username",dlg.loader().get<wxString>(PropField1).ToStdString());
    ns.data().setValue(p,"Password",dlg.loader().get<wxString>(PropField2).ToStdString());
    ns.data().setValue(p,"Host",dlg.loader().get<wxString>(PropField3).ToStdString());
    ns.data().setValue(p,"Port",dlg.loader().get<int>(PropField4));
    ns.data().setValue(p,"Item",dlg.loader().get<wxString>(PropField5).ToStdString());
}
/*!
 * \brief load
 * \param dlg
 * \param ns
 * \param p
 */
void NODEFLOW::MqttOutputNodeType::load(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
{
    NodeType::load(dlg,ns,p);
    dlg->addStringProperty("Username",ns.data().getValue<std::string>(p,"Username"));
    dlg->addStringProperty("Password",ns.data().getValue<std::string>(p,"Password"));
    dlg->addStringProperty("Host",ns.data().getValue<std::string>(p,"Host"));
    dlg->addIntProperty("Port",ns.data().getValue<int>(p,"Port"),1,64000);
    dlg->addStringProperty("Item",ns.data().getValue<std::string>(p,"Item"));
}
/*!
 * \brief save
 * \param dlg
 * \param ns
 * \param p
 */
void NODEFLOW::MqttOutputNodeType::save(NODEFLOW::WebProperties *dlg,NODEFLOW::NodeSet &ns,MRL::PropertyPath p)
{
    NodeType::save(dlg,ns,p);
    std::string  v =  dlg->getString(NODEFLOW::PropField1);
    ns.data().setValue(p,"Username",v);
    v = dlg->getString(NODEFLOW::PropField2);
    ns.data().setValue(p,"Password",v);
    v = dlg->getString(NODEFLOW::PropField3);
    ns.data().setValue(p,"Host",v);
    ns.data().setValue(p,"Port",dlg->getInt(NODEFLOW::PropField4));
    v = dlg->getString(NODEFLOW::PropField5);
    ns.data().setValue(p,"Item",v);
}


