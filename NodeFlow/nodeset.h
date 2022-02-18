#ifndef NODESET_H
#define NODESET_H
#include <MrlLib/mrllib.h>
#include <MrlLib/variantpropertytree.h>
#include <MrlLib/sqlitedb.h>
//
#include "node.h"
#include "nodetype.h"
#include "edge.h"
//
#include <functional>


namespace NODEFLOW
{

typedef std::function<void(NodePtr &)> NodeIteratorFunc;
typedef std::function<void(EdgePtr &)> EdgeIteratorFunc;


/*!
     * \brief The NodeSet class
     * Node /  Edge add and delete shoudl be done via the NodeSet not the maps
     */
class NodeSet
{
    //
    // Set of nodes
    //
    NodePtrMap _nodes;
    // set of edges
    EdgePtrMap _edges;
    //
    unsigned _edgeId = 100;
    unsigned _nodeId = 100;
    //
    NodePtr _null;
    //
    MRL::VariantPropertyTree _data;
    //
public:
    NodeSet() {}
    virtual ~NodeSet() {}

    void clear(); // clear everything

    void clearNodesSelected() // clear node selection flag
    {
        for(auto i = _nodes.begin(); i != _nodes.end(); i++)
        {
            NodePtr &n = i->second;
            n->setSelected(false);
        }
    }


    void clearEdgesSelected() // clear edge selection flag
    {
        for(auto i = _edges.begin(); i != _edges.end(); i++)
        {
            EdgePtr &e = i->second;
            e->setSelected(false);
        }
    }

    void clearSelected() // clear all selected items
    {
        clearNodesSelected();
        clearEdgesSelected();
    }
    /*!
     * \brief nodes
     * \return
     */
    NodePtrMap & nodes() {
        return  _nodes;
    }



    /*!
     * \brief edges
     * \return
     */
    EdgePtrMap & edges() {
        return _edges;
    }

    /*!
     * \brief toNodePath
     * \param id
     * \param path
     */
    void toNodePath(unsigned id, MRL::PropertyPath &path)
    {
        NodePtr & p = findNode(id);
        if(p)
        {
            p->toPath(path);
        }
    }
    //
     MRL::VariantPropertyTree & data() { return  _data;}
    /*!
     * \brief addNode of given type
     * \param s - either const std::string & or size_t
     */
     template<typename T>
    unsigned addNode(T s, int x = 0, int y = 0, unsigned id = 0)
    {

        NodeType *t = NodeType::find(s);
        if(t)
        {
            if(id == 0) id = _nodeId++;
            Node * n =  t->createNode(id);
            if(n)
            {
                _nodes[id] = std::move(std::unique_ptr<Node>(n));
                MRL::PropertyPath p;
                p.push_back("Nodes");
                p.push_back(n->sid());
                data().setValue("Id",id);
                n->setLocation(x,y);
                data().setValue("X",x);
                data().setValue("Y",y);
                data().setString("Name","Name");
                n->save(*this);
            }
        }
        return 0;
    }


    /*!
     * \brief deleteNode
     * \param id
     */
    void deleteNode(unsigned id)
    {
        NodePtr &n = findNode(id);
        if(n)
        {
            MRL::PropertyPath p;
            n->toPath(p);
            data().remove(p);
            disconnectNode(id);
            _nodes.erase(id);
        }
    }
    /*!
     * \brief find
     * \param id
     * \return
     */
    NodePtr & findNode(unsigned id)
    {
        auto i = _nodes.find(id);
        if(i != _nodes.end())
        {
            return i->second;
        }
        return _null;
    }
    /*!
     * \brief addConnect
     * \param nodeFrom
     * \param out
     * \param nodeTo
     * \param in
     */
    unsigned addConnect(unsigned nodeFrom, unsigned out, unsigned nodeTo, unsigned in);

    /*!
     * \brief connect
     * \param nodeFrom
     * \param out
     * \param nodeTo
     * \param in
     */
    void connect(unsigned id, unsigned nodeFrom, unsigned out, unsigned nodeTo, unsigned in);
    /*!
     * \brief disconnect
     * \param id
     */
    void disconnect(unsigned id); // remove an edge - remove from database remove from tree
    //
    // Edge database
    //
    unsigned search(const Edge &e) // look for duplicate
    {
        for(auto i = _edges.begin(); i != _edges.end(); i++)
        {
            EdgePtr &j = i->second;
            if(e == *j) return i->first; // return the edge id
        }
        return 0;
    }

    void disconnectNode(unsigned id);

    EdgePtr _nullEdge;
    EdgePtr & findEdge(unsigned id)
    {
        auto i = _edges.find(id);
        if(i != _edges.end()) return i->second;
        return _nullEdge;
    }

    void enumNodes(const NodeIteratorFunc &f);
    void enumEdges(const EdgeIteratorFunc &f);

    //
    virtual void step();
    virtual void start();
    virtual void stop();
    //
    virtual void load();
    virtual void save();

};
}
#endif // NODESET_H
