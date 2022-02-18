#ifndef EDGE_H
#define EDGE_H
#include "connectionpoint.h"
#include <MrlLib/variantpropertytree.h>
#include <memory>
#include <map>
#include <set>
namespace NODEFLOW
{
/*!
     * \brief The Edge class
     */
class NodeSet;
    class Edge
    {
        unsigned _id = 0;
        ConnectionPoint _from; // source of connection
        ConnectionPoint _to; // destination of connection
        bool _selected = false;
    public:
        Edge() {}
        Edge(const Edge &e) = default;
        Edge(unsigned id, const ConnectionPoint &f, const ConnectionPoint &t) : _id(id),_from(f),_to(t) {}
        Edge(unsigned id, unsigned nodeFrom, unsigned out, unsigned nodeTo, unsigned in) : _id(id),_from(nodeFrom,out),_to(nodeTo,in) { }
        virtual ~Edge() {}
        const ConnectionPoint & from() const { return  _from;}
        const ConnectionPoint &to() const { return _to;}
        unsigned id() const { return _id;}
        void setId(unsigned i) { _id = i;}
        bool operator == (const Edge &e) const
        {
            return (_from == e._from) && (_to == e._to);
        }

        void toPath(MRL::PropertyPath &p) // path in tree to a node
        {
            p.clear();
            p.push_back("Edges");
            p.push_back(std::to_string(id()));
        }

        /*!
         * \brief selected
         * \return
         */
        bool selected() const
        {
            return _selected;
        }
        /*!
         * \brief setSelected
         * \param f
         */
        void setSelected(bool f = true)
        {
            _selected = f;
        }
        virtual void load(NodeSet &);
        virtual void save(NodeSet &);

    };
    typedef std::unique_ptr<Edge> EdgePtr;
    typedef std::map<unsigned,EdgePtr> EdgePtrMap;
    typedef std::set<unsigned> EdgeIdSet;

}
#endif // EDGE_H
