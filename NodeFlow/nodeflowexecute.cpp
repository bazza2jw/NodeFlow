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
#include "nodeflowexecute.h"

/*!
 * \brief NODEFLOW::NodeFlowExecute::NodeFlowExecute
 */
NODEFLOW::NodeFlowExecute::NodeFlowExecute()
{

}

bool NODEFLOW::NodeFlowExecute::load(const std::string &s)
{
    _set.clear();
    bool ret =  _set.data().load(s);
    _set.start(); // initalise the node set
    return ret;
}


void NODEFLOW::NodeFlowExecute::process()
{
    // read from the input queue and set the input value then call step. Call step even if no value is waiting.

}
