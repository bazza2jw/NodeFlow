#include "functiontypenode.h"

#include <fstream>
#include <istream>

/*!
 * \brief NODEFLOW::FunctionTypeNode::load
 * \param file
 * \return true on success
 */
bool NODEFLOW::FunctionTypeNode::load(const std::string file)
{
    try
    {
        std::ifstream is(file);
        if(is.is_open())
        {
            Json::Value v;
            is >> v;
            //_config.fromJson(v);
        }
    }
    catch(...)
    {

    }
    return false;
}

//
// instance the operators
//
#define BINARY_OP(n,t,o)\
static  NODEFLOW::BinaryTypeNode<t,t>  n##_instance (#n, [](t a, t b) { return a o b;});

BINARY_OP(FloatAdd,double,+)
BINARY_OP(FloatSubtract,double,-)
BINARY_OP(FloatMultiply,double,*)
// divide needs validation
static  NODEFLOW::BinaryTypeNode<double,double> DivideInstance("FloatDivide", [](double a, double b) { return (b != 0.0) ?(a / b):0.0;});


BINARY_OP(IntAdd,int,+)
BINARY_OP(IntSubtract,int,-)
BINARY_OP(IntMultiply,int,*)
//
BINARY_OP(IntAnd,int, &)
BINARY_OP(IntOr,int,|)
BINARY_OP(IntXOR,int, ^)
//
BINARY_OP(LogicalAnd,bool, &&)
BINARY_OP(LogicalOr,bool,||)

//
// divide needs validation
static  NODEFLOW::BinaryTypeNode<double,double> IntDivideInstance("IntDivide", [](int a, int b) { return (b != 0.0) ?(a / b):0.0;});


#define RELATIONAL_OP(n,t,o)\
static  NODEFLOW::BinaryTypeNode<t,bool>  n##_instance (#n, [](t a, t b) { return a o b;});
//
RELATIONAL_OP(GreaterThan,double, >)
RELATIONAL_OP(LessThan,double, <)
RELATIONAL_OP(Equals,double, ==)
RELATIONAL_OP(NotEquals,double, !=)
//
RELATIONAL_OP(IntGreaterThan,int, >)
RELATIONAL_OP(IntLessThan,int, <)
RELATIONAL_OP(IntEquals,int, ==)
RELATIONAL_OP(IntNotEquals,int, !=)
//
static  NODEFLOW::UnaryTypeNode<double,double>  MinusInstance("Minus",[](double a){ return -a;});
static  NODEFLOW::UnaryTypeNode<bool,bool>  NotInstance("Not",[](bool a){return !a;});
static  NODEFLOW::UnaryTypeNode<unsigned,unsigned>  ComplimentInstance("Compliment",[](unsigned a){return ~a;});
//
// Type conversion
//
static  NODEFLOW::UnaryTypeNode<double,int>  DoubleToIntInstance("DoubleToInt",[](double a){ return int(a);});
static  NODEFLOW::UnaryTypeNode<int,double>  IntToDoubleInstance("IntToDouble",[](int a){ return double(a);});
static  NODEFLOW::UnaryTypeNode<int,bool>    IntToBoolInstance("IntToBool",[](int a){ return (a != 0);});
static  NODEFLOW::UnaryTypeNode<double,bool> DoubleToBoolInstance("DoubleToBool",[](double a){ return (a != 0);});
//
// TBD Strings and time





