# NodeFlow

This is in development. 

Node based flow framework in C++ similar in concept to Node Red 

This is hopefully useful - procedural node based programming has lots of applications

Possibly just use as examples of C++, wxWidgets and Wt



Features
========

Multiple inputs are supported 

Multiple outputs are supported

Plugins are supported 

Node Sets (flows) can be nodes

Relational operators nodes can act as data packet routers 

Binary operators can have the B input defaulted otherwise both A and B inputs have to be triggered to generate an output

There are data processing nodes, such as rolling statisics.

Data packets (JSON Values) can be routed by topic. Topic can be changed.


Program Structure
=================

Generally the functions are performed by NodeType derived classes. The other classes hold data. In some cases classes derived form Node are required to store extra data items.


The NodeSet class is the graph of the nodes (Node) and the connections (Edge). The NodeSet is the execution context.  To run a flow  the NodeSet is executed/processed (by calling NodeSet::step). The NodeSet::start function is called to initialise all nodes. A NodeSet has a Variant Property Tree to store the node layout and node properties and anything else that springs to mind.

All nodes receive the step call then the trigger call. 

Data / event source nodes emit data on the trigger call. Data is sent from outputs to connected inputs by calling post. The data is received and processed by the process function

Data is passed as Json Values. Each value has a "Topic" attribute and a "Payload" attribute (cf Node Red). Other attributes can be added.

The node set execution flow is:

Call NodeSet::start to initialise the nodes

while 
    
    call NodeSet::step. This first invokes NodeType::step for each node. If true is returned by the NodeTypeStep::step the node is ready to be triggered (it is the left most node of the flow). The NodeType::trigger function is now called for these nodes. 

    Data is sent to conencted nodes using the NodeType::post function and processed by the NodeType::process function. The NodeType::process function then posts the results to output(s). This repeats (recurses) until the end of the flow.
    Flows are executed from start to finish. Flows must not block. The step function can be used to drive state machines. Do not assume any given flow (or branch)  will run before or after any other flow.
 

    read the set output queue and process the packets. The output messages can be passed to the UI components, databases etc based on the topic attribute. In the GUI run mode the messages from the node set are displayed in the trace panel.

endwhile


GUI
===

The GUI is a graphical node set editor. Drag and drop nodes form the list on the left on to canvas. Connect nodes by left-down on an  output and draging to an input. Right click on a node ot get a context menu that includes editing properties and deleting nodes. To delete connections right click on a input or output and select delete connections.

Node connections have colour coded type. Nodes must be of the same type (Bool, Integer, Float and String) to be connected. There is an Any type that can connect to any other type. 

Web Interface
=============

The web version of the Node Editor has been written using Wt. The port is 8083. 


Building
========

Use cmake to build. 

The GUI is wxWidgets based. However changing to Qt or Wt (for web based UI) should not be complex.

The MrlLib from the OpcServant project is required. Boost is required.

Look at the plugins for how to extend the node set and how to implement functions

It is expected to be able to embed Node Flow in other applications as a visual alternative to scripting and configuration. 





