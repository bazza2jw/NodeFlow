
#include "nodedraw.h"
#include "nodeset.h"
#include <set>

unsigned NODEFLOW::NodeDrawBitmap::_gSession = 100;
/*!
 * \brief NODEFLOW::NodeDraw::NodeDraw
 */
NODEFLOW::NodeDrawWx::NodeDrawWx (NodeSet &s) :
    _normalPen(*wxBLACK_PEN),
    _selectPen(*wxBLUE,2),
    _erasePen(*wxWHITE,2),
    _eraseBrush(*wxWHITE_BRUSH),
    _connectionFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL),
    _titleFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL),
    _nodeSet(s)

{

}

/*!
 * \brief NODEFLOW::NodeDraw::draw
 * \param dc
 * \param rect
 */
void NODEFLOW::NodeDrawWx::draw(wxDC &dc, const wxRect &area)
{
    _edgeDrawSet.clear();
    // draw the nodes
    for(auto i = _nodeSet.nodes().begin(); i != _nodeSet.nodes().end(); i++)
    {
        NODEFLOW::NodePtr &n = i->second;
        wxRect r = n->layout().rect();
        r.SetPosition(n->location());
        if(r.Intersects(area))
        {
            if(n) drawNode(dc,n);
        }
    }
    //
    // now draw the edges
    //
    for(auto i = _edgeDrawSet.begin(); i != _edgeDrawSet.end(); i++)
    {
        NODEFLOW::EdgePtr &e = _nodeSet.findEdge(*i);
        if(e) drawEdge(dc,e);
    }
}


/*!
 * \brief CanvasWindows::drawNode
 */

void NODEFLOW::NodeDrawWx::drawNode(wxDC &dc, NODEFLOW::NodePtr &n)
{
    //
    wxDCPenChanger pc(dc,(n->selected())?_selectPen:_normalPen );
    wxBrush b(n->colour());
    wxDCBrushChanger bc(dc,b);
    //
    NODEFLOW::NodeType *t = NODEFLOW::NodeType::find(n->type());
    const NODEFLOW::NodeLayout &l = t->nodeLayout(n->id());
    wxRect rn = l.rect();
    //
    rn.SetPosition(n->location());
    n->getNodeEdges(_edgeDrawSet); // set of edges to draw
    dc.DrawRectangle(rn);
    //
    {
        dc.SetPen(*wxBLACK_PEN);
        dc.SetFont(_connectionFont);
        wxRect tr(rn.GetLeft(),rn.GetTop() - (CONNECTION_SIZE + 3), rn.GetWidth(), CONNECTION_SIZE);
        dc.DrawLabel(t->name(), tr, wxALIGN_CENTER_HORIZONTAL);
        // Now draw the connectors
        if(l.inputCount())
        {
            for(size_t i = 0; i < l.inputCount(); i++)
            {
                wxRect r = l.input(i);
                r.Offset(n->location());
                //
                const NODEFLOW::Connection &c = t->inputs()[i];
                wxRect lr((r.GetLeft() + 2), r.GetTop() - (CONNECTION_SIZE + 2), rn.GetWidth()/2, CONNECTION_SIZE );
                dc.DrawLabel(c.name(),lr,wxALIGN_LEFT);

                switch(l.inputType(i))
                {
                case NODEFLOW::Bool :
                    dc.SetBrush(*wxRED_BRUSH);
                    break;
                case NODEFLOW::Integer:
                    dc.SetBrush(*wxGREEN_BRUSH);
                    break;
                case NODEFLOW::Float:
                    dc.SetBrush(*wxYELLOW_BRUSH);
                    break;
                case NODEFLOW::String:
                    dc.SetBrush(*wxLIGHT_GREY_BRUSH);
                    break;
                default:
                    dc.SetBrush(*wxWHITE_BRUSH);
                    break;
                }
                dc.DrawRectangle(r);
            }
        }

        if(l.outputCount())
        {
            for(size_t i = 0; i < l.outputCount(); i++)
            {
                wxRect r = l.output(i);
                r.Offset(n->location());
                //
                wxRect lr(r.GetLeft() - rn.GetWidth()/2, r.GetTop() - CONNECTION_SIZE, rn.GetWidth()/2, CONNECTION_SIZE );
                const NODEFLOW::Connection &c = t->outputs()[i];
                dc.DrawLabel(c.name(),lr,wxALIGN_RIGHT);
                //
                switch(l.outputType(i))
                {
                case NODEFLOW::Bool :
                    dc.SetBrush(*wxRED_BRUSH);
                    break;
                case NODEFLOW::Integer:
                    dc.SetBrush(*wxGREEN_BRUSH);
                    break;
                case NODEFLOW::Float:
                    dc.SetBrush(*wxYELLOW_BRUSH);
                    break;
                case NODEFLOW::String:
                    dc.SetBrush(*wxLIGHT_GREY_BRUSH);
                    break;

                default:
                    dc.SetBrush(*wxWHITE_BRUSH);
                    break;
                }

                dc.DrawRectangle(r);
            }
        }
    }

}

/*!
 * \brief drawEdge
 */
void NODEFLOW::NodeDrawWx::drawEdge( wxDC &dc,NODEFLOW::EdgePtr &e)
{
    if(e)
    {
        if(e->selected())
        {
            dc.SetPen(_selectPen);
        }
        else
        {
            dc.SetPen(_normalPen);
        }
        //
        // Get the start and end points
        NODEFLOW::NodePtr & fn = _nodeSet.findNode(e->from().node());
        wxRect beg = fn->layout().output(e->from().id());
        beg.Offset(fn->location());
        NODEFLOW::NodePtr & tn = _nodeSet.findNode(e->to().node());
        wxRect end = tn->layout().input(e->to().id());
        end.Offset(tn->location());
        //
        drawSpline(dc,beg.GetPosition(),end.GetPosition());
    }
}







