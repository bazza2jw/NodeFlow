/*
 * Copyright (C) 2022 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */

#ifndef NODEFLOWEXECUTE_H
#define NODEFLOWEXECUTE_H
//
#include "nodetype.h"
#include "nodeset.h"
#include "node.h"
//
namespace NODEFLOW
{
/*!
     * \brief The NodeFlowExecute class
     */
    class NodeFlowExecute
    {
        NodeSet _set;
        VALUEQUEUE _in;
    public:
        NodeFlowExecute();
        bool load(const std::string &s);
        void process();
        void post(const VALUE &v ) { _in.push(v);}
    };
}
#endif // NODEFLOWEXECUTE_H
