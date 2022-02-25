#include "webcanvas.h"
#include <Wt/WMenuItem.h>
#include <Wt/WMenu.h>
#include <Wt/WPopupMenu.h>
#include "webaddobjectdialog.h"
#include "webdialog.h"
/*!
 * \brief NODEFLOW::WebCanvas::WebCanvas
 */
NODEFLOW::WebCanvas::WebCanvas() : _draw(_nodes)
{
    setAttributeValue("oncontextmenu", "event.cancelBubble = true; event.returnValue = false; return false;");
    // make the connections
    mouseDragged().connect(this, &WebCanvas::mouseDrag);
    mouseWentDown().connect(this, &WebCanvas::mouseDown);

}

/*!
 * \brief NODEFLOW::WebCanvas::~WebCanvas
 */
NODEFLOW::WebCanvas::~WebCanvas()
{
}
/*!
 * \brief NODEFLOW::WebCanvas::paintEvent
 * \param paintDevice
 */
void NODEFLOW::WebCanvas::paintEvent(Wt::WPaintDevice *paintDevice) {
    Wt::WPainter painter(paintDevice);
    painter.setRenderHint(Wt::RenderHint::Antialiasing);
    Wt::WBrush b(Wt::StandardColor::Red);
    _draw.draw(painter);
    switch(_state)
    {
    case NodeSet::NODE_SELECT:
    {
        wxRect r = _startHit._currentLayout.rect();
        Wt::WRectF rr(r.GetLeft(),r.GetTop(),r.GetWidth(),r.GetHeight());
        painter.drawRect(rr);
    }
    break;
    case NodeSet::INPUT_SELECT:
    case NodeSet::OUTPUT_SELECT:
    {
        wxPoint b(_startpoint.x(),_startpoint.y());
        wxPoint e(_currentpoint.x(),_currentpoint.y());
        _draw.drawSpline(painter,b,e);
    }
        break;

    default:
        break;
    }
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
                            break;
                        case 1:
                            // Properties
                            break;
                        default:
                            break;
                        }
                    }
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
                            // delete the current node
                            break;
                        case 1:
                            // Properties
                            break;
                        default:
                            break;
                        }
                    }

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
                            // delete the current node
                            break;
                        case 1:
                            // Properties
                            break;
                        default:
                            break;
                        }
                    }
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
                        NODEFLOW::WebDialogBase::showDialog<>(this,p);
                        update(Wt::PaintFlag::Update);
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
                update(Wt::PaintFlag::Update);
                break;
            case NODEFLOW::NodeSet::NODE_SELECT:
            {
                _startHit._node->setLocation(pc);
                update(Wt::PaintFlag::Update);
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





