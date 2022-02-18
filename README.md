# NodeFlow

Node based flow framework in C++ similar in concept to Node Red

This framework is intended to support both flows and calculations.

The big differences between Node Flow and Node Red / VisualScriptEngine are:

Multiple inputs are supported

Multiple outputs are supported

Both operation modes are supported, flow and calculate.

Plugins are supported 


Flows are left to right from outputs to inputs.

Caclulations are right to left from inputs to outputs. Before an output value can be calculated all on the dependant inputs have to be evaluated. (cf VisualScriptEngine)


Generally the functions are performed by NodeType derived classes. The other classes hold data.


The NodeSet class is the graph of the nodes (Node) and the connections (Edge). The NodeSet is the execution context.  To run a flow or calculation the NodeSet is executed/processed.

The GUI is wxWidgets based. However changing to Qt or Wt (for web based UI) should not be complex. It should be converting to the equivalent classes, most of the code being unchanged.

