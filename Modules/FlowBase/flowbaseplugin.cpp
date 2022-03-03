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

#include "flowbaseplugin.h"

void addFunctionNodes();
void addValueNodes();
void addControlNodes();
void addDataProcessing();
void addOutputs();

namespace NODEFLOW
{
/*!
     * \brief The FlowBasePlugin class
     */
    class FlowBasePlugin
    {
    public:
        FlowBasePlugin()
        {
            addFunctionNodes();
            addValueNodes();
            addControlNodes();
            addDataProcessing();
            addOutputs();
        }

    } _instance;
}
