# NodeFlow

This is in development - may work or not

Node based flow framework in C++ similar in concept to Node Red 

This framework is intended to support both flows and calculations.

The big differences between Node Flow and Node Red are:

Multiple inputs are supported

Multiple outputs are supported

Both operation modes are supported, flow and calculate. Though flow (left to right) is being developed first

Plugins are supported 

Node Sets (flows) can be nodes

Relational operators nodes can act as data packet routers 

Binary operators can have the B input defaulted otherwise both A and B inputs have to be triggered to generate an output

Data packets (JSON Values) can be routed by topic. Topic can be changed.


Flows are left to right from outputs to inputs.


Generally the functions are performed by NodeType derived classes. The other classes hold data.

The NodeSet class is the graph of the nodes (Node) and the connections (Edge). The NodeSet is the execution context.  To run a flow or calculation the NodeSet is executed/processed (by calling step).

All nodes receive the step call then the trigger call. 

Data / event source nodes emit data on the trigger call. Data is sent from outputs to connected inputs by calling post. The data is received and processed by the process function

The GUI is wxWidgets based. However changing to Qt or Wt (for web based UI) should not be complex. It should be converting to the equivalent classes, most of the code being unchanged.


The MrlLib from the OpcServant project is required

Look at the plugins for how to extend the node set

It is expected to embed Node Flow in other applications as a visual alternative to scripting and configuration





