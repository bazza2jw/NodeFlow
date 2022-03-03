/*
 * Copyright (C) 2018 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */
#ifndef MQTTCONNECTION_H
#define MQTTCONNECTION_H
#include <MrlLib/mrllib.h>
#include <mosquittopp.h>
#include <memory>
#include <map>
#include "node.h"
namespace NODEFLOW
{

    class MqttConnection;
    /*!
     * \brief The MqttCommand class
     * Handles an incoming request keyed by an "action" item in the message
     */
    class MqttCommand
    {
    public:
        MqttCommand(const std::string &);
        virtual ~MqttCommand(){}
        virtual void action(MqttConnection &/*connection*/, const std::string &/*topic*/, Json::Value &/*payload*/) {} // do the action
    };

    typedef std::map<std::string,MqttCommand *> MqttCommandMap;
    /*!
     * \brief The MqttConnection class
     */
    class MqttConnection : public mosqpp::mosquittopp
    {
            bool _ok = false;
            static MqttCommandMap _map;
        public:
        /*!
             * \brief MqttConnection
             * \param id
             * \param host
             * \param port
             */
            MqttConnection(const char *id = "");
            /*!
              */
            ~MqttConnection();
            /*!
             * \brief on_connect
             * \param rc
             */
            void on_connect(int rc);
            /*!
             * \brief on_message
             * \param message
             */
            void on_message(const struct mosquitto_message *message);
            /*!
             * \brief on_subscribe
             * \param mid
             * \param qos_count
             * \param granted_qos
             */
            void on_subscribe(int mid, int qos_count, const int *granted_qos);

            /*!
             * \brief ok
             * \return true if OK
             */
            bool ok() const {return _ok;}

            /*!
             * \brief map
             * \return
             */
            static MqttCommand * find(const std::string &name)
            {
                if(_map.find(name) != _map.end()) return _map[name];
                return nullptr;
            }
            /*!
             * \brief addCommand
             * \param name
             * \param handler
             */
            static void addCommand(const std::string &name, MqttCommand *handler)
            {
                _map[name] = handler;
            }

            virtual void defaultParse(VALUE &){} // parse a JSON payload that does not have an opcode defined
            virtual void defaultParse(const std::string &) {} // parse a non-JSON message

    };

    typedef std::unique_ptr<MqttConnection> MqttConnectionRef;
}

#endif // MQTTCONNECTION_H
