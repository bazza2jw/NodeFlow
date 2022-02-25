#include "nodedraw.h"
#include "nodeset.h"
#include <set>


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
void NODEFLOW::NodeDrawWx::draw(wxDC &dc)
{
    _edgeDrawSet.clear();
    // draw the nodes
    for(auto i = _nodeSet.nodes().begin(); i != _nodeSet.nodes().end(); i++)
    {
        NODEFLOW::NodePtr &n = i->second;
        if(n) drawNode(dc,n);
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



// ********************************** Wt draw interface **************************************


NODEFLOW::NodeDrawWt::NodeDrawWt (NodeSet &s) :
    _normalPen(Wt::StandardColor::Black),
    _selectPen(Wt::StandardColor::Blue),
    _erasePen(Wt::StandardColor::White),
    _eraseBrush(Wt::StandardColor::White),
    _boolBrush(Wt::StandardColor::Red),
    _intBrush(Wt::StandardColor::Green),
    _floatBrush(Wt::StandardColor::Yellow),
    _stringBrush(Wt::StandardColor::DarkGray),
    _anyBrush(Wt::StandardColor::White),
    _connectionFont(Wt::FontFamily::Monospace),
    _titleFont(Wt::FontFamily::Monospace),
    _nodeSet(s)

{
    _selectPen.setWidth(2);
    _connectionFont.setSize(Wt::FontSize::XSmall);
    _connectionFont.setSize(Wt::FontSize::Small);

}

/*!
 * \brief NODEFLOW::NodeDrawWt::drawNode
 * \param dc
 * \param n
 */
void NODEFLOW::NodeDrawWt::drawNode(Wt::WPainter &dc, NodePtr &n)
{
    //
    dc.setPen(_normalPen);
    dc.setFont(_connectionFont);
    NODEFLOW::NodeType *t = NODEFLOW::NodeType::find(n->type());
    const NODEFLOW::NodeLayout &l = t->nodeLayout(n->id());
    wxRect rn = l.rect();
    Wt::WColor c(n->colour().Red(),n->colour().Green(),n->colour().Blue());
    Wt::WBrush br(c);
    rn.SetPosition(n->location());
    n->getNodeEdges(_edgeDrawSet); // set of edges to draw
    dc.fillRect(double(rn.GetX()), double(rn.GetY()) , double(rn.GetWidth()), double(rn.GetHeight()),br);
    //
    {
        dc.setPen(Wt::StandardColor::Black);
        dc.setFont(_connectionFont);
        // text box
        Wt::WRectF tr( double(rn.GetLeft()) ,
                       double(rn.GetTop() - (CONNECTION_SIZE + 3)),
                       double(rn.GetWidth()),
                       double(CONNECTION_SIZE));
        dc.drawText (tr, Wt::AlignmentFlag::Center , t->name());
        // Now draw the connectors
        if(l.inputCount())
        {
            for(size_t i = 0; i < l.inputCount(); i++)
            {
                wxRect r = l.input(i);
                r.Offset(n->location());
                Wt::WRectF cr(r.GetLeft(),r.GetTop(),r.GetWidth(),r.GetHeight());
                //
                const NODEFLOW::Connection &c = t->inputs()[i];
                Wt::WRectF lr((r.GetLeft() + 2), r.GetTop() - (CONNECTION_SIZE + 2), rn.GetWidth()/2, CONNECTION_SIZE );
                dc.drawText(lr,Wt::AlignmentFlag::Left, c.name());

                switch(l.inputType(i))
                {
                case NODEFLOW::Bool :
                    dc.fillRect(cr,_boolBrush);
                    break;
                case NODEFLOW::Integer:
                    dc.fillRect(cr,_intBrush);
                    break;
                case NODEFLOW::Float:
                    dc.fillRect(cr,_floatBrush);
                    break;
                case NODEFLOW::String:
                    dc.fillRect(cr,_stringBrush);
                    break;
                default:
                    dc.fillRect(cr,_anyBrush);
                    break;
                }
            }
        }

        if(l.outputCount())
        {
            for(size_t i = 0; i < l.outputCount(); i++)
            {
                wxRect r = l.output(i);
                r.Offset(n->location());
                Wt::WRectF cr(r.GetLeft(),r.GetTop(),r.GetWidth(),r.GetHeight());
                //
                Wt::WRectF lr(r.GetLeft() - rn.GetWidth()/2, r.GetTop() - CONNECTION_SIZE, rn.GetWidth()/2, CONNECTION_SIZE );
                const NODEFLOW::Connection &c = t->outputs()[i];
                dc.drawText(lr,Wt::AlignmentFlag::Right, c.name());
                //
                switch(l.outputType(i))
                {
                case NODEFLOW::Bool :
                    dc.fillRect(cr,_boolBrush);
                    break;
                case NODEFLOW::Integer:
                    dc.fillRect(cr,_intBrush);
                    break;
                case NODEFLOW::Float:
                    dc.fillRect(cr,_floatBrush);
                    break;
                case NODEFLOW::String:
                    dc.fillRect(cr,_stringBrush);
                    break;
                default:
                    dc.fillRect(cr,_anyBrush);
                    break;
                }

            }
        }
    }

}
/*!
 * \brief NODEFLOW::NodeDrawWt::drawEdge
 * \param dc
 * \param e
 */
void NODEFLOW::NodeDrawWt::drawEdge(Wt::WPainter &dc, EdgePtr &e)
{
    if(e)
    {
        if(e->selected())
        {
            dc.setPen(_selectPen);
        }
        else
        {
            dc.setPen(_normalPen);
        }
        // Get the start and end points
        NODEFLOW::NodePtr & fn = _nodeSet.findNode(e->from().node());
        wxRect beg = fn->layout().output(e->from().id());
        beg.Offset(fn->location());
        NODEFLOW::NodePtr & tn = _nodeSet.findNode(e->to().node());
        wxRect end = tn->layout().input(e->to().id());
        end.Offset(tn->location());
        drawSpline(dc,beg.GetPosition(),end.GetPosition());
    }
}
/*!
 * \brief NODEFLOW::NodeDrawWt::drawSpline
 * \param dc
 * \param beg
 * \param end
 */
void NODEFLOW::NodeDrawWt::drawSpline(Wt::WPainter &dc,wxPoint beg, wxPoint end)
{
    Wt::WPainterPath pt;
    pt.quadTo(beg.x,beg.y,end.x,end.y);
    dc.drawPath(pt); // draw the spline
}
/*!
 * \brief NODEFLOW::NodeDrawWt::draw
 * \param dc
 */
void NODEFLOW::NodeDrawWt::draw(Wt::WPainter &dc)
{
    _edgeDrawSet.clear();
    // draw the nodes
    for(auto i = _nodeSet.nodes().begin(); i != _nodeSet.nodes().end(); i++)
    {
        NODEFLOW::NodePtr &n = i->second;
        if(n) drawNode(dc,n);
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



