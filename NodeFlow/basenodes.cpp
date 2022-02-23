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

#include "basenodes.h"
#include "nodetype.h"
#include "nodesettype.h"


static
/*!
 * \brief The LoadBaseNodes class
 */
class LoadBaseNodes
{
public:
    LoadBaseNodes()
    {
       NODEFLOW::NodeType::addType<NODEFLOW::BaseInput>("BaseInput");
       NODEFLOW::NodeType::addType<NODEFLOW::BaseOutput>("BaseOutput");
       NODEFLOW::NodeType::addType<NODEFLOW::BaseConnector>("BaseConnector");
       NODEFLOW::NodeType::addType<NODEFLOW::NodeSetType>("NodeSet");
       NODEFLOW::NodeType::addType<NODEFLOW::NodeSetOutputNodeType>("NodeSetOutput");
       NODEFLOW::NodeType::addType<NODEFLOW::NodeSetInputNodeType>("NodeSetInput");
    }
} _instance __attribute__ ((init_priority (2000)));
