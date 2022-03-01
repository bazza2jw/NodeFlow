#include "webcanvas.h"
#include <Wt/WMenuItem.h>
#include <Wt/WMenu.h>
#include <Wt/WPopupMenu.h>
#include "webaddobjectdialog.h"
#include "NodeFlow/webdialog.h"
/*!
 * \brief NODEFLOW::WebCanvas::WebCanvas
 */
NODEFLOW::WebCanvas::WebCanvas() :_canvasSize(0,0,_width,_height),
    _normalPen(Wt::StandardColor::Black),
    _selectPen(Wt::StandardColor::Blue),
    _boolBrush(Wt::StandardColor::Red),
    _intBrush(Wt::StandardColor::Green),
    _floatBrush(Wt::StandardColor::Yellow),
    _stringBrush(Wt::StandardColor::DarkGray),
    _anyBrush(Wt::StandardColor::White),

    _connectionFont(Wt::FontFamily::Monospace),
    _titleFont(Wt::FontFamily::Monospace)
{
    setAttributeValue("oncontextmenu", "event.cancelBubble = true; event.returnValue = false; return false;");
    // make the connections
    mouseDragged().connect(this, &WebCanvas::mouseDrag);
    mouseWentDown().connect(this, &WebCanvas::mouseDown);
    mouseWentUp().connect(this, &WebCanvas::mouseUp);
    _connectionFont.setSize(Wt::FontSize::XSmall);
    _titleFont.setSize(Wt::FontSize::Small);
}

/*!
 * \brief NODEFLOW::WebCanvas::~WebCanvas
 */
NODEFLOW::WebCanvas::~WebCanvas()
{
}
/*!
 * \brief NODEFLOW::WebCanvas::readSet
 * \param s
 */
void NODEFLOW::WebCanvas::readSet(const std::string &s)
{
    _nodes.clear();
    _fileName = s;
    if(wxFile::Exists(s))
    {
        // proceed loading the file chosen by the user;
        _nodes.data().load(_fileName);
        _nodes.load();
    }
    update();
}
/*!
 * \brief NODEFLOW::WebCanvas::save
 */
void NODEFLOW::WebCanvas::save()
{
        _nodes.save();
        _nodes.data().save(_fileName);
}
/*!
 * \brief NODEFLOW::WebCanvas::saveAs
 * \param f
 */
void NODEFLOW::WebCanvas::saveAs(const std::string &f)
{
    _fileName = f;
    save();
}

/*!
 * \brief NODEFLOW::WebCanvas::drawNode
 * \param dc
 * \param n
 */
void NODEFLOW::WebCanvas::drawNode(Wt::WPainter &dc, NodePtr &n)
{
    dc.save();
    Wt::WColor nc(n->colour().Red(),n->colour().Green(),n->colour().Blue());
    Wt::WBrush b(nc);
    dc.setPen(n->selected()?_selectPen:_normalPen);
    dc.setBrush(b);
    //
    NODEFLOW::NodeType *t = NODEFLOW::NodeType::find(n->type());
    const NODEFLOW::NodeLayout &l = t->nodeLayout(n->id());
    Wt::WRectF rn(n->location().x,n->location().y,l.rect().GetWidth(),l.rect().GetHeight());
    n->getNodeEdges(_edgeDrawSet); // set of edges to draw
    dc.drawRect(rn);
    //
    {
        dc.setPen(_normalPen);
        dc.setFont(_connectionFont);
        Wt::WRectF tr(rn.left(),rn.top() - (CONNECTION_SIZE + 3), rn.width(), CONNECTION_SIZE);
        dc.drawText(tr,Wt::AlignmentFlag::Center,t->name());
        // Now draw the connectors
        if(l.inputCount())
        {
            for(size_t i = 0; i < l.inputCount(); i++)
            {
                wxRect r = l.input(i);
                r.Offset(n->location());
                //
                const NODEFLOW::Connection &c = t->inputs()[i];
                Wt::WRectF lr((r.GetLeft() + 2), r.GetTop() - (CONNECTION_SIZE + 2), rn.width()/2, CONNECTION_SIZE );
                dc.drawText(lr, Wt::AlignmentFlag::Left,c.name());
                //
                dc.drawRect(r.GetLeft(),r.GetTop(),r.GetWidth(),r.GetHeight());
                //
                switch(l.inputType(i))
                {
                case NODEFLOW::Bool :
                    dc.fillRect(r.GetLeft(),r.GetTop(),r.GetWidth(),r.GetHeight(),_boolBrush);
                    break;
                case NODEFLOW::Integer:
                    dc.fillRect(r.GetLeft(),r.GetTop(),r.GetWidth(),r.GetHeight(),_intBrush);
                    break;
                case NODEFLOW::Float:
                    dc.fillRect(r.GetLeft(),r.GetTop(),r.GetWidth(),r.GetHeight(),_floatBrush);
                    break;
                case NODEFLOW::String:
                    dc.fillRect(r.GetLeft(),r.GetTop(),r.GetWidth(),r.GetHeight(),_stringBrush);
                    break;
                default:
                    dc.fillRect(r.GetLeft(),r.GetTop(),r.GetWidth(),r.GetHeight(),_anyBrush);
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
                //
                const NODEFLOW::Connection &c = t->outputs()[i];
                Wt::WRectF lr((r.GetLeft() + 2), r.GetTop() - (CONNECTION_SIZE + 2), rn.width()/2, CONNECTION_SIZE );
                dc.drawText(lr, Wt::AlignmentFlag::Right,c.name());
                //
                dc.drawRect(r.GetLeft(),r.GetTop(),r.GetWidth(),r.GetHeight());
                //
                switch(l.outputType(i))
                {
                case NODEFLOW::Bool :
                    dc.fillRect(r.GetLeft(),r.GetTop(),r.GetWidth(),r.GetHeight(),_boolBrush);
                    break;
                case NODEFLOW::Integer:
                    dc.fillRect(r.GetLeft(),r.GetTop(),r.GetWidth(),r.GetHeight(),_intBrush);
                    break;
                case NODEFLOW::Float:
                    dc.fillRect(r.GetLeft(),r.GetTop(),r.GetWidth(),r.GetHeight(),_floatBrush);
                    break;
                case NODEFLOW::String:
                    dc.fillRect(r.GetLeft(),r.GetTop(),r.GetWidth(),r.GetHeight(),_stringBrush);
                    break;
                default:
                    dc.fillRect(r.GetLeft(),r.GetTop(),r.GetWidth(),r.GetHeight(),_anyBrush);
                    break;
                }
            }
        }
    }
    dc.restore();

}
/*!
 * \brief NODEFLOW::WebCanvas::drawEdge
 * \param dc
 * \param e
 */
void NODEFLOW::WebCanvas::drawEdge(Wt::WPainter &dc, EdgePtr &e)
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
        //
        // Get the start and end points
        NODEFLOW::NodePtr & fn = _nodes.findNode(e->from().node());
        wxRect beg = fn->layout().output(e->from().id());
        beg.Offset(fn->location());
        NODEFLOW::NodePtr & tn = _nodes.findNode(e->to().node());
        wxRect end = tn->layout().input(e->to().id());
        end.Offset(tn->location());
        //
        drawSpline(dc,beg.GetPosition(),end.GetPosition());
    }

}
/*!
 * \brief NODEFLOW::WebCanvas::draw
 * \param dc
 * \param area
 */
void NODEFLOW::WebCanvas::draw(Wt::WPainter &dc, const wxRect &area)
{
    dc.save();
    _edgeDrawSet.clear();
    // draw the nodes
    for(auto i = _nodes.nodes().begin(); i != _nodes.nodes().end(); i++)
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
        NODEFLOW::EdgePtr &e = _nodes.findEdge(*i);
        if(e) drawEdge(dc,e);
    }
    dc.restore();
}

/*!
 * \brief NODEFLOW::WebCanvas::paintEvent
 * \param paintDevice
 */
void NODEFLOW::WebCanvas::paintEvent(Wt::WPaintDevice *paintDevice) {
    Wt::WPainter painter(paintDevice);
    painter.setRenderHint(Wt::RenderHint::Antialiasing);
    draw(painter,_canvasSize);

    switch(_state)
    {
    case NodeSet::NODE_SELECT:
    {
        wxRect r = _startHit._currentLayout.rect();
        r.SetPosition(wxPoint(int(_currentpoint.x()),int(_currentpoint.y())));
        Wt::WRectF rr(r.GetLeft(),r.GetTop(),r.GetWidth(),r.GetHeight());
        painter.drawRect(rr);
    }
    break;
    case NodeSet::INPUT_SELECT:
    case NodeSet::OUTPUT_SELECT:
    {
        wxPoint b(_startpoint.x(),_startpoint.y());
        wxPoint e(_currentpoint.x(),_currentpoint.y());
        drawSpline(painter,b,e);
    }
    break;

    default:

        break;
    }
}

/*!
 * \brief NODEFLOW::WebCanvas::drawSpline
 * \param dc
 * \param beg
 * \param end
 */
void NODEFLOW::WebCanvas::drawSpline(Wt::WPainter &dc,wxPoint beg, wxPoint end)
{
    wxPoint pts[4];
    int dx = end.x - beg.x ;
    int dy = end.y - beg.y;
    wxPoint dp(dx/2,0);
    wxPoint cc(0,CONNECTION_SIZE/2);
    pts[0] = beg + cc;
    pts[1] = beg + dp;
    pts[2] = end - dp;
    pts[3] = end + cc;

    Wt::WPainterPath pt;
    pt.moveTo(double(beg.x),double(beg.y));
    pt.cubicTo(pts[1].x,pts[1].y,pts[2].x,pts[2].y,end.x,end.y);
    dc.drawPath(pt); // draw the spline
}


/*!
 * \brief NODEFLOW::WebCanvas::mouseDown
 * \param e
 */
void NODEFLOW::WebCanvas::mouseDown(const Wt::WMouseEvent& e) {
    Wt::Coordinates c = e.widget();
    if(e.button() == Wt::MouseButton::Left)
    {
        if(editMode())
        {
            if(_state ==NODEFLOW::NodeSet::NONE)
            {
                _currentpoint = Wt::WPointF( c.x, c.y ) ;
                _nodes.clearSelected(); // clear all selections
                _startHit.clear();
                wxPoint pc(c.x,c.y);
                _state = _nodes.hitTest(pc,_startHit);
                if(_state != NODEFLOW::NodeSet::NONE)
                {
                    _startpoint = _currentpoint;
                    _currentHit = _startHit;
                }
            }
        }
    }
    else
    {
        if(editMode())
        {
            union {
                void *pv;
                int a;
            };
            // context menu
            _currentpoint = Wt::WPointF( c.x, c.y ) ;
            wxPoint pc(c.x,c.y);
            _state = _nodes.hitTest(pc,_startHit);
            _currentHit = _startHit;
            //
            if(_currentHit._node)
            {
                switch(_state)
                {
                case NodeSet::NODE_SELECT:
                {
                    Wt::WPopupMenu m;
                    Wt::WMenuItem *i;
                    i = m.addItem(std::make_unique<Wt::WMenuItem>("Delete Node"));
                    i->setData((void *)2);
                    i = m.addItem(std::make_unique<Wt::WMenuItem>("Properties"));
                    i->setData((void *)1);
                    i = m.addItem(std::make_unique<Wt::WMenuItem>("Cancel"));
                    i->setData((void *)0);
                    i = m.exec(e);
                    if(i)
                    {
                        pv = i->data();
                        switch(a)
                        {
                        case 2:
                            // delete the current node
                            _nodes.deleteNode(_currentHit._nodeId);
                            break;
                        case 1:
                            // Properties
                        {
                            _currentHit._node->save(_nodes); // save curent location
                            _currentHit._node->nodeType()->properties(this,_nodes,_currentHit._nodeId);
                            _currentHit._node->load(_nodes); // load any changed attiributes

                        }
                            break;
                        default:
                            break;
                        }
                    }
                    update();
                }
                break;
                case NodeSet::INPUT_SELECT:
                {
                    Wt::WPopupMenu m;
                    Wt::WMenuItem *i;
                    i = m.addItem(std::make_unique<Wt::WMenuItem>("Delete Input"));
                    i->setData((void *)2);
                    i = m.addItem(std::make_unique<Wt::WMenuItem>("Cancel"));
                    i->setData((void *)0);
                    i = m.exec(e);
                    if(i)
                    {
                        pv = i->data();
                        switch(a)
                        {
                        case 2:
                            _nodes.removeEdgesFromInput(_currentHit._nodeId,_currentHit._connectorSelect);
                            break;
                        default:
                            break;
                        }
                    }
                    update();

                }
                break;
                case NodeSet::OUTPUT_SELECT:
                {
                    Wt::WPopupMenu m;
                    Wt::WMenuItem *i;
                    i = m.addItem(std::make_unique<Wt::WMenuItem>("Delete Output"));
                    i->setData((void *)2);
                    i = m.addItem(std::make_unique<Wt::WMenuItem>("Cancel"));
                    i->setData((void *)0);
                    i = m.exec(e);
                    if(i)
                    {
                        pv = i->data();
                        switch(a)
                        {
                        case 2:
                            _nodes.removeEdgesFromOutput(_currentHit._nodeId,_currentHit._connectorSelect);
                            break;
                        default:
                            break;
                        }
                    }
                    redraw();
                }
                break;

                default:
                    break;
                }
            }
            else
            {
                // add item
                Wt::WPopupMenu m;
                Wt::WMenuItem *i;
                i = m.addItem(std::make_unique<Wt::WMenuItem>("Add"));
                i->setData((void *)2);
                i = m.addItem(std::make_unique<Wt::WMenuItem>("Cancel"));
                i->setData((void *)0);
                i = m.exec(e);
                if(i)
                {
                    pv = i->data();
                    if(pv)
                    {
                        std::unique_ptr<NODEFLOW::WebAddObjectForm> p = std::make_unique<NODEFLOW::WebAddObjectForm>(pc,_nodes);
                        p->redrawRequest().connect(this,&NODEFLOW::WebCanvas::redraw);
                        NODEFLOW::WebDialogBase::showDialog<>(this,p);
                    }
                }
            }
        }
        // context menu
        _state = NODEFLOW::NodeSet::NONE;
    }
}
/*!
 * \brief NODEFLOW::WebCanvas::mouseUp
 * \param e
 */
void NODEFLOW::WebCanvas::mouseUp(const Wt::WMouseEvent& e) {
    Wt::Coordinates c = e.widget();
    if(editMode())
    {
        if ( _state != NODEFLOW::NodeSet::NONE)
        {
            _currentpoint = Wt::WPointF( c.x, c.y );
            wxPoint pc(c.x,c.y);
            wxPoint sc(_startpoint.x(),_startpoint.y());

            switch(_state)
            {
            case NODEFLOW::NodeSet::INPUT_SELECT:
            case NODEFLOW::NodeSet::OUTPUT_SELECT:
                _nodes.makeConnectionSelect(pc, sc, _state, _startHit);
                update();
                break;
            case NODEFLOW::NodeSet::NODE_SELECT:
            {
                _startHit._node->setLocation(pc);
                update();
            }
            break;
            default:
                break;
            }
        }
    }
    _state = NODEFLOW::NodeSet::NONE;

}
/*!
 * \brief NODEFLOW::WebCanvas::mouseDrag
 * \param e
 */
void NODEFLOW::WebCanvas::mouseDrag(const Wt::WMouseEvent& e) {
    Wt::Coordinates c = e.widget();
    if(editMode())
    {
        if ( _state != NODEFLOW::NodeSet::NONE)
        {
            _currentpoint = Wt::WPointF( c.x, c.y );
        }
    }
    update();
}





