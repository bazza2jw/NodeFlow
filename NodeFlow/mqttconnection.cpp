/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of NodeFlow. NodeFlow C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#include "mqttconnection.h"
#include <MrlLib/json/json.h>

NODEFLOW::MqttCommandMap NODEFLOW::MqttConnection::_map; // command map

/*!
 * \brief NODEFLOW::MqttCommand::MqttCommand
 */
NODEFLOW::MqttCommand::MqttCommand(const std::string &s)
{
    // self register
    NODEFLOW::MqttConnection::addCommand(s,this);
}

/*!
 * \brief NODEFLOW::MqttConnection::MqttConnection
 */
NODEFLOW::MqttConnection::MqttConnection( const char *id)
    : mosqpp::mosquittopp(id)
{

}
/*!
 * \brief NODEFLOW::MqttConnection::~MqttConnection
 */
NODEFLOW::MqttConnection::~MqttConnection()
{

}
/*!
 * \brief NODEFLOW::MqttConnection::on_connect
 * \param rc
 */
void NODEFLOW::MqttConnection::on_connect(int rc)
{
    //wxLogDebug("Connected with code %d.\n", rc);
    _ok = (rc== 0);
}
/*!
 * \brief NODEFLOW::MqttConnection::on_message
 * \param message
 */
void NODEFLOW::MqttConnection::on_message(const struct mosquitto_message *message)
{
    // received a message
    Json::Value v;
    std::string payload( static_cast<const char *>(message->payload),message->payloadlen);
    std::string topic(message->topic);
    if(MRL::stringToJson(payload,v))
    {
        Json::Value &op = v["action"];
        if(!op.isNull())
        {
            // find the command an execute it
            std::string o = op.asString();
            MqttCommand *c = find(o);
            if(c)
            {
                c->action(*this,topic,v);
            }
        }
        else
        {
            // no op defined
            defaultParse(v);
        }
    }
    else
    {
        // not JSON so something else
        defaultParse(payload);
    }
}
/*!
 * \brief NODEFLOW::MqttConnection::on_subscribe
 * \param mid
 * \param qos_count
 * \param granted_qos
 */
void NODEFLOW::MqttConnection::on_subscribe(int /*mid*/, int /*qos_count*/, const int */*granted_qos*/)
{
   // wxLogDebug("Subscription succeeded.\n");
}

