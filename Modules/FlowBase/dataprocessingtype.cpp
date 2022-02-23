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

#include "dataprocessingtype.h"

void addDataProcessing()
{
    NODEFLOW::NodeType::addType<NODEFLOW::ScaleTypeNode>("Scale"); // scaler and offset
    NODEFLOW::NodeType::addType<NODEFLOW::WindowThresholdTypeNode>("HiLoThreshold"); // high low range test
    NODEFLOW::NodeType::addType<NODEFLOW::StatisticsNodeType>("Statistics"); // rolling buffer stats
    NODEFLOW::NodeType::addType<NODEFLOW::HiHiLoLoThresholdTypeNode>("HiHiLoLo");// action / alert levels
}
