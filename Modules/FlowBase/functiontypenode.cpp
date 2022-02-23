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

#include "functiontypenode.h"
#include <fstream>
#include <istream>



/*!
 * \brief addFunctionNodes
 */
void addFunctionNodes()
{
//
//
    NODEFLOW::NodeType::addType<NODEFLOW::TopicChangeTypeNode>("TopicSetter");
    NODEFLOW::NodeType::addType<NODEFLOW::TopicFilterTypeNode>("TopicFilter");
    NODEFLOW::NodeType::addType<NODEFLOW::FunctionTypeNode>("Function");
    NODEFLOW::NodeType::addType<NODEFLOW::RbeTypeNode>("Report By Exception");
//
// instance the operators
//
#define BINARY_OP(n,t,o,p)\
{ auto pt = new  NODEFLOW::BinaryTypeNode<t,t,p,p>(#n, [](t a, t b) { return a o b;}); pt->setup();}

    BINARY_OP(FloatAdd,double,+,NODEFLOW::Float)
    BINARY_OP(FloatSubtract,double,-,NODEFLOW::Float)
    BINARY_OP(FloatMultiply,double,*,NODEFLOW::Float)
// divide needs validation
    {
        auto pt = new  NODEFLOW::BinaryTypeNode<double,double,NODEFLOW::Float,NODEFLOW::Float>("FloatDivide", [](double a, double b) {
            return (b != 0.0) ?(a / b):0.0;
        });
        pt->setup();
    }
//
    BINARY_OP(IntAdd,int,+,NODEFLOW::Integer)
    BINARY_OP(IntSubtract,int,-,NODEFLOW::Integer)
    BINARY_OP(IntMultiply,int,*,NODEFLOW::Integer)
//
    BINARY_OP(IntAnd,int, &,NODEFLOW::Integer)
    BINARY_OP(IntOr,int,|,NODEFLOW::Integer)
    BINARY_OP(IntXOR,int, ^,NODEFLOW::Integer)
//
    BINARY_OP(LogicalAnd,bool, &&,NODEFLOW::Bool)
    BINARY_OP(LogicalOr,bool,||,NODEFLOW::Bool)
//
// divide needs validation
    {
        auto pt = new    NODEFLOW::BinaryTypeNode<int,int,NODEFLOW::Integer,NODEFLOW::Integer>("IntDivide", [](int a, int b) {
            return (b != 0) ?(a / b):0;
        });
        pt->setup();
    }
    {
        auto pt = new    NODEFLOW::BinaryTypeNode<int,int,NODEFLOW::Integer,NODEFLOW::Integer>("IntModulus", [](int a, int b) {
            return (b != 0) ?(a % b):0;
        });
        pt->setup();
    }
//
#define RELATIONAL_OP(n,t,o,p)\
{auto pt = new NODEFLOW::BinaryRelationalTypeNode<t,p> (#n, [](t a, t b) { return a o b;}); pt->setup();}
//
    RELATIONAL_OP(GreaterThan,double, >,NODEFLOW::Float)
    RELATIONAL_OP(LessThan,double, <,NODEFLOW::Float)
    RELATIONAL_OP(Equals,double, ==,NODEFLOW::Float)
    RELATIONAL_OP(NotEquals,double, !=,NODEFLOW::Float)
//
    RELATIONAL_OP(IntGreaterThan,int, >,NODEFLOW::Integer)
    RELATIONAL_OP(IntLessThan,int, <,NODEFLOW::Integer)
    RELATIONAL_OP(IntEquals,int, ==,NODEFLOW::Integer)
    RELATIONAL_OP(IntNotEquals,int, !=,NODEFLOW::Integer)
//
    {
        auto pt = new NODEFLOW::BinaryRelationalTypeNode<std::string,NODEFLOW::String>("StringEquals", [](const std::string & a, const std::string & b) {
            return a == b;
        });
        pt->setup();
    }
    {
        auto pt = new NODEFLOW::BinaryRelationalTypeNode<std::string,NODEFLOW::String>("StringNotEquals", [](const std::string & a, const std::string & b) {
            return a != b;
        });
        pt->setup();
    }
//
    {
        auto pt = new NODEFLOW::UnaryTypeNode<double,double,NODEFLOW::Float,NODEFLOW::Float>("Minus",[](double a) {
            return -a;
        });
        pt->setup();
    }

    {
        auto pt = new NODEFLOW::UnaryTypeNode<int,int,NODEFLOW::Integer,NODEFLOW::Integer>("IntMinus",[](int a) {
            return -a;
        });
        pt->setup();
    }


    {
        auto pt = new NODEFLOW::UnaryTypeNode<bool,bool,NODEFLOW::Bool,NODEFLOW::Bool>("Not",[](bool a) {
            return !a;
        });
        pt->setup();
    }
    {
        auto pt = new NODEFLOW::UnaryTypeNode<unsigned,unsigned,NODEFLOW::Integer,NODEFLOW::Integer>("Compliment",[](unsigned a) {
            return ~a;
        });
        pt->setup();
    }
//
// Type conversion
//
    {
        auto pt = new NODEFLOW::UnaryTypeNode<double,int,NODEFLOW::Float,NODEFLOW::Integer>("DoubleToInt",[](double a) {
            return int(a);
        });
        pt->setup();
    }
    {
        auto pt = new NODEFLOW::UnaryTypeNode<int,double,NODEFLOW::Integer,NODEFLOW::Float>("IntToDouble",[](int a) {
            return double(a);
        });
        pt->setup();
    }
    {
        auto pt = new NODEFLOW::UnaryTypeNode<int,bool,NODEFLOW::Integer,NODEFLOW::Bool>("IntToBool",[](int a) {
            return (a != 0);
        });
        pt->setup();
    }
    {
        auto pt = new NODEFLOW::UnaryTypeNode<double,bool,NODEFLOW::Float,NODEFLOW::Bool>("DoubleToBool",[](double a) {
            return (a != 0);
        });
        pt->setup();
    }
//
// TBD Strings and time
}




