#ifndef NODEDRAW_H
#define NODEDRAW_H
#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#   include "wx/wx.h"
#endif
#include <memory>
#include <map>
#include <wx/gdicmn.h>
#include <wx/dcmemory.h>
#include <wx/graphics.h>

#include "nodetype.h"
#include "edge.h"


namespace NODEFLOW
{

class NodeSet;

typedef enum
{
    PenForeground, PenBackground, PenSelected
} PenType;

/*!
 * \brief The NodeDraw class
 */
class NodeDraw
{
    EdgeIdSet _edgeDrawSet; // only draw an edge once if necesary
    void getNodeEdges(NodePtr &);
public:
    NodeDraw ();
    NodeDraw (const NodeDraw & rhs) = delete;
    virtual ~NodeDraw () {}
    virtual void draw(NodeSet &s , wxRect &rect);
    virtual void begin() {}
    virtual void end() {}
    virtual void drawNode(NodeSet &s ,NodePtr &, bool selected = false);
    virtual void drawEdge(NodeSet &s , EdgePtr &);
    virtual void eraseNode(NodeSet &s ,NodePtr &);
    virtual void setPen( PenType) {}


};




}
#endif // NODEDRAW_H
