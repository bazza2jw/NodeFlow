#include "nodeset.h"

/*!
 * \brief NODEFLOW::NodeSet::addConnect
 * \param nodeFrom
 * \param out
 * \param nodeTo
 * \param in
 */
unsigned NODEFLOW::NodeSet::addConnect(unsigned nodeFrom, unsigned out, unsigned nodeTo, unsigned in)
{
    try
    {
        unsigned equal = 0;
        Edge e(0, nodeFrom,out,nodeTo,in);
        //
        // check database - is there a duplicate
        equal = search(e);
        //
        if(equal == 0) // no duplicate ?
        {
            NodePtr &s = findNode(nodeFrom); // get the nodes
            if(s)
            {
                NodeType *st = NodeType::find(s->type());
                if(st) // can always connect from an output
                {
                    if(out < st->outputs().size())
                    {

                        NodePtr &d = findNode(nodeTo);
                        if(d)
                        {
                            NodeType *dt = NodeType::find(d->type());
                            if(in < dt->inputs().size())
                            {
                                const Connection &sc = st->outputs()[out];
                                const Connection &dc = dt->inputs()[in];
                                // connection types must match
                                //
                                // although JSON values are used many operations are type sensitive
                                // this is the type of the payload
                                // a value is a map of values not just the payload
                                // there are type conversion nodes
                                //
                                //
                                if((dc.type() == Any) || (sc.type() == Any) || (sc.type() == dc.type())) // is the input of Any type or does the output type equal the input type
                                {
                                    // how many connections
                                    size_t to = 0;
                                    ItemListPtr &ip =  d->inputs()[in];
                                    if(ip) to = ip->size();
                                    if((dc.mode() == Multiple) ||(to == 0)) // either multiple connections are allowed or there are none
                                    {
                                        _edgeId++;
                                        connect(_edgeId,nodeFrom,out,nodeTo, in); // create the edge
                                        return _edgeId;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    CATCH_DEF
    return 0;
}

/*!
 * \brief NODEFLOW::NodeSet::disconnect
 * \param id
 */
void NODEFLOW::NodeSet::disconnect(unsigned id) // remove an edge - remove from database remove from tree
{
    try
    {
        auto i = _edges.find(id);
        if(i != _edges.end())
        {
            MRL::PropertyPath p;
            EdgePtr &e = i->second;
            e->toPath(p);
            //
            NodePtr &d = findNode(e->from().node());
            NodePtr &s = findNode(e->to().node()); // get the nodes
            //
            //
            if(s && d)
            {
                // remove the references to the edges from the nodes
                //
                if( s->outputs().size() )
                {
                    ItemListPtr &o = s->outputs()[e->from().id()]; // list of connections to this output
                    for(auto j = o->begin(); j != o->end(); j++ )
                    {
                        if(*j == id)
                        {
                            o->erase(j);
                            break;
                        }
                    }
                }
                //
                if(s->inputs().size())
                {
                    ItemListPtr &i = s->inputs()[e->to().id()]; // list of connections to this input
                    for(auto j = i->begin(); j != i->end(); j++ )
                    {
                        if(*j == id)
                        {
                            i->erase(j);
                            break;
                        }
                    }
                }
                //
                _edges.erase(id); // remove the edge
                data().remove(p); // remove the edge from the data tree
            }
        }
    }
    CATCH_DEF
}

/*!
 * \brief NODEFLOW::NodeSet::disconnectNode
 * \param id
 */
void NODEFLOW::NodeSet::disconnectNode(unsigned id)
{
    std::vector<unsigned> dl; //delete list
    // remove all connections for a given node
    for(auto i = _edges.begin(); i != _edges.end(); i++)
    {
        EdgePtr &e = i->second;
        if(e)
        {
            if((e->from().node() == id) || (e->to().node() == id))
            {
                dl.push_back(e->id()); // add to list
            }
        }
    }
    for(auto i = dl.begin(); i != dl.end(); i++)
    {
        disconnect(*i);
    }
}

/*!
 * \brief NODEFLOW::NodeSet::connect
 * \param id
 * \param nodeFrom
 * \param out
 * \param nodeTo
 * \param in
 */
void NODEFLOW::NodeSet::connect(unsigned id, unsigned nodeFrom, unsigned out, unsigned nodeTo, unsigned in)
{
    _edges[id] = std::make_unique<Edge>(id, nodeFrom,out,nodeTo,in);
    EdgePtr &p = findEdge(id);
    p->save(*this);
    //
    NodePtr &d = findNode(nodeTo);
    NodePtr &s = findNode(nodeFrom); // get the nodes
    //
    ItemListPtr &o = s->outputs()[out]; // list of connections to this output
    ItemListPtr &i = d->inputs()[in]; // list of connections to this input
    //
    if(o) o->push_back(id); // add edge
    if(i) i->push_back(id); // add edge
}

/*!
 * \brief NODEFLOW::NodeSet::step
 */
void  NODEFLOW::NodeSet::step(const VALUE &in)
{
    try
    {
        setInValue(in);
        while(!outValue().empty()) outValue().pop() ; // clear the queue
        //
        std::vector<unsigned> tl; // trigger list
        // for each node call the step function for the node's type
        // the step function is for periodic processing
        for(auto i = _nodes.begin(); i != _nodes.end(); i++)
        {
            NodePtr &n = i->second;
            if(n)
            {
                NodeType *t = NodeType::find(n->type());
                if(t)
                {
                    if(t->step(*this,n))
                    {
                        tl.push_back(n->id());
                    }
                }
            }
        }
        // trigger as required
        if(tl.size() > 0)
        {
            for(auto i = tl.begin(); i != tl.end(); i++)
            {
                NodePtr &n = findNode(*i);
                if(n)
                {
                    NodeType *t = NodeType::find(n->type());
                    if(t)
                    {
                        // triggered nodes execute flows to completion - flows should not block - use the node step function to drive state machines and the like
                        // once all nodes have triggered (that can trigger)
                        t->trigger(*this,n);
                    }
                }
            }
        }
        // the result is  queue of output values
    }
    CATCH_DEF
}

/*!
 * \brief NODEFLOW::NodeSet::enumNodes
 * \param f
 */
void NODEFLOW::NodeSet::enumNodes(const NodeIteratorFunc &f)
{
    try
    {
        for(auto i = _nodes.begin(); i != _nodes.end(); i++)
        {
            NodePtr &n = i->second;
            if(n) f(n);
        }
    }
    CATCH_DEF
}

/*!
 * \brief NODEFLOW::NodeSet::enumEdges
 * \param f
 */
void NODEFLOW::NodeSet::enumEdges(const EdgeIteratorFunc &f)
{
    for(auto i = _edges.begin(); i != _edges.end(); i++)
    {
        EdgePtr &e = i->second;
        if(e) f(e);
    }
}

/*!
 * \brief NODEFLOW::NodeSet::start
 */
void NODEFLOW::NodeSet::start()
{
    enumNodes( [&](NodePtr &n) {
        if(n)
        {
            NodeType *t = NodeType::find(n->type());
            if(t)
            {
                t->start(*this,n);
            }
        }
    });
}
/*!
 * \brief NODEFLOW::NodeSet::stop
 */
void NODEFLOW::NodeSet::stop()
{
    enumNodes( [&](NodePtr &n) {
        if(n)
        {
            NodeType *t = NodeType::find(n->type());
            if(t)
            {
                t->stop(*this,n);
            }
        }
    });
}
/*!
 * \brief NODEFLOW::NodeSet::load
 */
void NODEFLOW::NodeSet::load()
{
    try
    {
        // Optimise the search and load
        // create the nodes
        std::list<std::string> nodeList;
        std::list<std::string> edgeList;
        //
        _nodeId = data().getInt("NodeId");
        _edgeId = data().getInt("EdgeId");
        //
        MRL::PropertyPath pn;
        pn.push_back("Nodes");
        MRL::PropertyPath pe;
        pe.push_back("Edges");
        //
        data().listChildren(pn,nodeList);
        data().listChildren(pe,edgeList);
        //
        // add the nodes
        for(auto i = nodeList.begin(); i != nodeList.end(); i++)
        {
            pn.push_back(*i);
            unsigned id = data().getValue<unsigned>(pn,"Id"); // get the id
            unsigned type = data().getValue<unsigned>(pn,"Type");
            NodeType *nt = NodeType::find(type);
            if(nt)
            {
                std::unique_ptr<Node> p(nt->createNode(id));
                _nodes[id] = std::move(p);
                NodePtr & n  = findNode(id);
                n->load(*this);
            }
            pn.pop_back();
        }

        // add the connections
        for(auto i = edgeList.begin(); i != edgeList.end(); i++)
        {
            pe.push_back(*i);
            unsigned id = data().getValue<unsigned>(pe,"Id");
            unsigned fromNode = data().getValue<unsigned>(pe,"FromNode");
            unsigned fromId = data().getValue<unsigned>(pe,"FromId");
            unsigned toNode = data().getValue<unsigned>(pe,"ToNode");
            unsigned toId = data().getValue<unsigned>(pe,"ToId");
            connect(id,fromNode,fromId,toNode,toId);
            pe.pop_back();
        }
    }
    CATCH_DEF
}
/*!
 * \brief NODEFLOW::NodeSet::save
 */
void NODEFLOW::NodeSet::save()
{
    try
    {
        data().setInt("NodeId",_nodeId);
        data().setInt("EdgeId",_edgeId);

        enumNodes([&](NodePtr &n) {
            n->save(*this);
        });
        enumEdges([&](EdgePtr &e) {
            e->save(*this);
        });
    }
    CATCH_DEF
}
/*!
 * \brief NODEFLOW::NodeSet::clear
 */
void NODEFLOW::NodeSet::clear()
{
    // clear the node dictionary
    _nodes.clear();
    // clear the edge dictionary
    _edges.clear();
    // clear the tree
    _data.clear();
    //
    _data.setBool("Nodes",false);
    _data.setBool("Edges",false);
    //
    _data.setInt("NodeId",100);
    _data.setInt("EdgeId",100);
    //
    _nodeId = _edgeId = 100;
}

