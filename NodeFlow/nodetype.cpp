#include "nodetype.h"
#include "nodeset.h"
#include "PropertiesEditorDialog.h"
//
//
NODEFLOW::NodeType::NodeTypeMap NODEFLOW::NodeType::_map __attribute__ ((init_priority (110))); // map name to factory
NODEFLOW::NodeType::NodeTypeIdMap NODEFLOW::NodeType::_mapId __attribute__ ((init_priority (110))); // map id to factory
unsigned NODEFLOW::NodeType::_msgId = 100;
//
//



/*!
 * \brief calculate
 * \param ns
 * \param nodeId
 * \param input id
 * \param data calculated value
 * \return true on success
 */
bool NODEFLOW::NodeType::calculate(NodeSet &ns, unsigned nodeId, unsigned id, VALUE &data )
{
    // evaluate each of the outputs attached to this input
    NodePtr  &n = ns.findNode(nodeId);
    VALUE r;
    if(n)
    {
        ItemListPtr &il = n->inputs()[id];
        if(il)
        {
            for(auto i = il->begin(); i != il->end(); i++)
            {
                EdgePtr &e = ns.findEdge(*i);
                if(e)
                {
                    VALUE d;
                    NodePtr &src = ns.findNode(e->from().node());
                    if(src)
                    {
                        NodeType *t = src->nodeType();
                        if(t->evaluate(ns,e->from().node(),e->from().id(),d))
                        {
                            // we have one value from one output connected to the input
                            processEvaluatedData(ns,n,id,d,r);
                        }
                    }
                }
            }
        }
        data = r;
    }
    return false;
}




/*!
 * \brief NODEFLOW::NodeType::evaluate
 * \param ns
 * \param nodeId
 * \param id output id
 * \param data
 * \return true on success
 */
bool NODEFLOW::NodeType::evaluate(NodeSet &ns, unsigned nodeId, unsigned id,  VALUE &data )
{
    // for the given output evaluate the inputs for the output
    NodePtr  &n = ns.findNode(nodeId);
    if(n)
    {
        if(n->calculatedData().find(id) != n->calculatedData().end())
        {
            data = n->calculatedData().at(id); // cached value
            return true;
        }
    }
    return false;
}

/*!
 * \brief process
 * \param nodeId this node to process the signal
 * \param id this is the input index
 * \return true on success
 */
bool NODEFLOW::NodeType::process(NodeSet &/*ns*/, unsigned /*nodeId*/, unsigned /*id*/, const VALUE &/*data*/)
{
    return false;
}
/*!
 * \brief post
 * \param nodeId this is the source node
 * \param id this is the output index
 * \return true on success
 */
bool NODEFLOW::NodeType::post(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data )
{
    NodePtr &n = ns.findNode(nodeId);
    if(n)
    {
        // get the list of edges attached to the output
        ItemListPtr &il = n->outputs()[id];
        for(auto i = il->begin(); i != il->end(); i++)
        {
            EdgePtr &e = ns.findEdge(*i);
            if(e)
            {
                NodePtr &dn = ns.findNode(e->to().node());
                if(dn)
                {
                    NodeType *t = NodeType::find(dn->type()); // get the destination type
                    if(t)
                    {
                        // process each connection
                        return t->process(ns,dn->id(),e->to().id(),data);
                    }
                }
            }
        }
    }
    return false;
}

/*!
 * \brief NODEFLOW::NodeType::setupConnections
 */
void NODEFLOW::NodeType::setupConnections()
{
    // setup the inputs
    inputs().resize(1);
    inputs()[0] = Connection("in",Multiple);
    // set up the outputs
    outputs().resize(1);
    outputs()[0] = Connection("out",Multiple);
}

/*!
 * \brief NODEFLOW::NodeType::setup
 * default -
 */
void NODEFLOW::NodeType::setup()
{
    //
    setupConnections();
    //
    _layout.setInputCount(inputs().size());
    _layout.setOutputCount(outputs().size());

    int h =  NODE_TITLE_BAR_HEIGHT + NODE_RECT_HEIGHT_MARGIN ; // title
    h += CONNECTION_SPACE * ((inputs().size() > outputs().size())? inputs().size():outputs().size());
    wxRect r(0,0,NODE_RECT_WIDTH,h);
    _layout.setRect(r);
    //
    // set the connection positions
    for(int i = 0 ; i < inputs().size(); i++)
    {
        _layout.addInput(wxPoint(0,NODE_TITLE_BAR_HEIGHT + i *CONNECTION_SPACE));
    }

    for(int i = 0; i < outputs().size(); i++)
    {
        _layout.addOutput(wxPoint(NODE_RECT_WIDTH - CONNECTION_SIZE,NODE_TITLE_BAR_HEIGHT + i *CONNECTION_SPACE));
    }
}


/*!
 * \brief NODEFLOW::NodeType::properties
 * \param n
 * \param nodeId
 * \return true if OK
 */
bool NODEFLOW::NodeType::properties(wxWindow *parent, NodeSet &n, unsigned nodeId)
{
    // property page
    NodePtr &np = n.findNode(nodeId);
    MRL::PropertyPath path;
    np->toPath(path);
    PropertiesEditorDialog dlg(parent,n.data(),path);
    dlg.loader().addStringProperty("Name","Name","Node Name");
    dlg.loader().addBoolProperty("Enable Node","Enable",true);
    return dlg.ShowModal() == wxID_OK;
}
