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

#include "edge.h"
#include "nodeset.h"
void NODEFLOW::Edge::load(NodeSet &s)
{
    MRL::VariantPropertyTree &t = s.data();
    MRL::PropertyPath p;
    toPath(p);
    //
    _from.setNode(t.getValue<unsigned>(p,"FromeNode"));
    _from.setId(t.getValue<unsigned>(p,"FromeId"));
    _to.setNode(t.getValue<unsigned>(p,"ToNode"));
    _to.setId(t.getValue<unsigned>(p,"ToId"));
    //
}

void NODEFLOW::Edge::save(NodeSet &s)
{
    MRL::VariantPropertyTree &t = s.data();
    MRL::PropertyPath p;
    toPath(p);
    t.setValue(p,"Id",id());
    t.setValue(p,"FromNode",_from.node());
    t.setValue(p,"FromId",_from.id());
    t.setValue(p,"ToNode",_to.node());
    t.setValue(p,"ToId",_to.id());
}
