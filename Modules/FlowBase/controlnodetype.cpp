#include "controlnodetype.h"

void NODEFLOW::IfElseNodeType::setupConnections()
{
    inputs().resize(1);
    inputs()[0] = Connection("in",Multiple);
    // set up the outputs
    outputs().resize(2);
    outputs()[0] = Connection("out",Multiple);
    outputs()[1] = Connection("else",Multiple);
}

void NODEFLOW::IfElseNodeType::setup()
{
    setupConnections();
    //
    nodeLayout().setInputCount(inputs().size());
    nodeLayout().setOutputCount(outputs().size());

    int h = NODE_TITLE_BAR_HEIGHT + NODE_RECT_HEIGHT_MARGIN ; // title
    h += CONNECTION_SPACE * ((inputs().size() > outputs().size())? inputs().size():outputs().size());
    wxRect r(0,0,NODE_RECT_WIDTH,h);
    nodeLayout().setRect(r);
    //
    // set the connection positions
    nodeLayout().addInput(wxPoint(0,NODE_TITLE_BAR_HEIGHT));
    nodeLayout().addOutput(wxPoint(NODE_RECT_WIDTH - CONNECTION_SIZE,NODE_TITLE_BAR_HEIGHT));
    nodeLayout().addOutput(wxPoint(NODE_RECT_WIDTH/2,h - CONNECTION_SIZE )); // the else goes on the bottom
}



void NODEFLOW::ElseIfNodeType::setup()
{
    setupConnections();
    //
    nodeLayout().setInputCount(inputs().size());
    nodeLayout().setOutputCount(outputs().size());
    //
    int h =  NODE_TITLE_BAR_HEIGHT + NODE_RECT_HEIGHT_MARGIN ; // title
    h += CONNECTION_SPACE * ((inputs().size() > outputs().size())? inputs().size():outputs().size());
    wxRect r(0,0,NODE_RECT_WIDTH,h);
    nodeLayout().setRect(r);
    //
    // set the connection positions top of node
    nodeLayout().addInput(wxPoint(NODE_RECT_WIDTH/2,CONNECTION_SIZE));

    nodeLayout().addOutput(wxPoint(NODE_RECT_WIDTH - CONNECTION_SIZE,NODE_TITLE_BAR_HEIGHT)); // if true message goes this way

    nodeLayout().addOutput(wxPoint(NODE_RECT_WIDTH/2,h - CONNECTION_SIZE )); // the else goes on the bottom

}
