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

#ifndef CONNECTIONPOINT_H
#define CONNECTIONPOINT_H

namespace NODEFLOW
{
typedef enum
{
    Output = 0,
    Input
} ConnectionPointType;

/*!
 * \brief The ConnectionPoint class
 * A connection point
 */
class ConnectionPoint
{
    unsigned _node = 0; // node id
    unsigned _id = 0; // connection point index (id)
public:
    ConnectionPoint() {}
    ConnectionPoint(unsigned n, unsigned id) : _node(n), _id(id) {}
    ConnectionPoint(const ConnectionPoint &) = default;
    virtual ~ConnectionPoint  () {}
    //
    unsigned node() const {
        return _node;
    }
    unsigned id() const {
        return _id;
    }
    void setNode(unsigned n) {
        _node = n;
    }
    void setId(unsigned i) {
        _id = i;
    }

    bool operator == (const ConnectionPoint &c) const { return (_node == c._node) && (_id == c._id);}
};

}


#endif // CONNECTIONPOINT_H
