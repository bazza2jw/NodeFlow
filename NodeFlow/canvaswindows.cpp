#include "canvaswindows.h"
#include <wx/menu.h>
#include <wx/filedlg.h>
#include <wx/stream.h>
#include <wx/wfstream.h>
#include <fstream>


/*!
 * \brief CanvasWindows::CanvasWindows
 * \param parent
 * \param w
 * \param h
 * \param scrollInterval
 */
CanvasWindows::CanvasWindows(wxWindow * parent, int w, int h, int scrollInterval) :
    wxScrolled<wxWindow>(parent,wxID_ANY),
    _width(w),
    _height(h),
    _scrollInterval(scrollInterval), // scrool interval and grid snap interval
    _dropTarget(new CanvasDropTarget(this)),
    //
    _normalPen(*wxBLACK_PEN),
    _selectPen(*wxBLUE,2),
    _erasePen(*wxWHITE,2),
    _eraseBrush(*wxWHITE_BRUSH),
    _connectionFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL),
    _titleFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL)

{
    SetScrollRate( scrollInterval, scrollInterval);
    SetVirtualSize( w, h);
    SetBackgroundColour( *wxWHITE );
    SetDropTarget 	( _dropTarget );
    //
    // Set up the event handlers
    //
    Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(CanvasWindows::onLeftDown), NULL, this);
    Connect(wxEVT_LEFT_UP, wxMouseEventHandler(CanvasWindows::onLeftUp), NULL, this);
    Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(CanvasWindows::onLeftDoubleClick), NULL, this);
    Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(CanvasWindows::onRightDown), NULL, this);
    Connect(wxEVT_RIGHT_UP, wxMouseEventHandler(CanvasWindows::onRightUp), NULL, this);
    Connect(wxEVT_RIGHT_DCLICK, wxMouseEventHandler(CanvasWindows::onRightDoubleClick), NULL, this);
    Connect(wxEVT_CHAR, wxKeyEventHandler(CanvasWindows::onChar), NULL, this);
    Connect(wxEVT_MOTION, wxMouseEventHandler(CanvasWindows::onMotion), NULL, this);
    Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(CanvasWindows::onWheel), NULL, this);
    Connect(wxEVT_MOUSE_CAPTURE_LOST, wxMouseCaptureLostEventHandler(CanvasWindows::OnCaptureLost), NULL, this);
    //
}

/*!
 * \brief CanvasWindows::readSet
 */
void CanvasWindows::readSet(const std::string &s)
{
    _nodes.clear();
    _fileName = s;
    if(wxFile::Exists(s))
    {
        // proceed loading the file chosen by the user;
        _nodes.data().load(_fileName);
        _nodes.load();
    }
    Refresh();
}

/*!
 * \brief CanvasWindows::open
 */
void CanvasWindows::open()
{
    wxFileDialog  openFileDialog(this, _("Open Node Flow file"), "", "", "NFL files (*.nfl)|*.nfl", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL) return;     // the user changed idea...
    std::string f = openFileDialog.GetPath().ToStdString();
    readSet(f);
}
/*!
 * \brief CanvasWindows::save
 */
void CanvasWindows::save()
{
    if(_fileName.empty())
    {
        saveAs();
    }
    else
    {
        _nodes.save();
        _nodes.data().save(_fileName);
    }
}
/*!
 * \brief CanvasWindows::saveAs
 */
void CanvasWindows::saveAs()
{
    wxFileDialog  saveFileDialog(this, _("Save Node Flow file"), "", "", "NFL files (*.nfl)|*.nfl", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_CANCEL) return;     // the user changed idea...
    _fileName = saveFileDialog.GetPath().ToStdString();
    _nodes.save();
    _nodes.data().save(_fileName);
}

/*!
 * \brief CanvasWindows::addNode
 * \param x
 * \param y
 * \param s
 */
void CanvasWindows::addNode(int x, int y,const std::string &s)
{
    // insert a node at the given location
    int xx, yy ;
    CalcUnscrolledPosition( x, y, &xx, &yy );
    _currentpoint = wxPoint( xx, yy ) ;
    // add it
    _nodes.addNode(s,xx,yy);
    // redraw
    Refresh();
}

/*!
 * \brief CanvasWindows::OnCaptureLost
 * \param event
 */
void CanvasWindows::OnCaptureLost(wxMouseCaptureLostEvent& event)
{
}
/*!
 * \brief CanvasWindows::onChar
 * \param event
 */
void CanvasWindows::onChar(wxKeyEvent& event)
{
    switch(event.GetKeyCode())
    {
    case WXK_DELETE:
    {
        if(_currentNode)
        {
            _nodes.deleteNode(_currentNode->id());
            _currentNode = nullptr;
            _state = IDLE;
        }
    }
    break;

    default:
        break;
    }
}
/*!
 * \brief CanvasWindows::onLeftDoubleClick
 * \param event
 */
void CanvasWindows::onLeftDoubleClick(wxMouseEvent& event)
{
}


void CanvasWindows::onMotion(wxMouseEvent &event)
{
    if(editMode())
    {
        if ( _state != IDLE)
        {
            int x,y, xx, yy ;
            event.GetPosition(&x,&y);
            CalcUnscrolledPosition( x, y, &xx, &yy );
            _currentpoint = wxPoint( xx, yy ) ;
            _rect.SetLeftTop(_currentpoint);

            wxClientDC dc( this ) ;
            PrepareDC( dc ) ;

            wxDCOverlay overlaydc( _overlay, &dc );
            overlaydc.Clear();
            dc.SetPen( wxPen( *wxLIGHT_GREY, 2 ) );
            dc.SetBrush( *wxTRANSPARENT_BRUSH );
            switch(_state)
            {
            case INPUT_SELECT:
                drawSpline(dc,_startpoint,_currentpoint);
                break;
            case OUTPUT_SELECT:
                drawSpline(dc,_startpoint,_currentpoint);
                break;
            case NODE_SELECT:
                dc.DrawRectangle( _rect );
                break;
            default:
                break;
            }
        }
        else
        {
            // set the tool tip if we get a hit
            int x,y,xx,yy ;
            event.GetPosition(&x,&y);
            CalcUnscrolledPosition( x, y, &xx, &yy );
            //
            _currentpoint = wxPoint( xx, yy ) ;
            //
            for(auto i = _nodes.nodes().begin(); i != _nodes.nodes().end(); i++)
            {
                NODEFLOW::NodePtr &n = i->second;
                wxPoint d = n->location() - _currentpoint;
                if((abs(d.x) < CONNECTION_SIZE) && (abs(d.y) < CONNECTION_SIZE))
                {
                    MRL::PropertyPath p;
                    p.push_back("Nodes");
                    p.push_back(n->sid());
                    std::string s = _nodes.data().getValue<std::string>(p,"Name"); // show the name and description
                    SetToolTip (s);
                }
            }
        }
    }
}

/*!
 * \brief CanvasWindows::getNodeEdges
 * \param n
 */
void CanvasWindows::getNodeEdges(NODEFLOW::NodePtr &n)
{
    if(n)
    {
        // draw the connected edges

        for(auto i = n->inputs().begin(); i != n->inputs().end(); i++)
        {
            NODEFLOW::ItemListPtr &il = *i;
            if(il)
            {
                for(auto j = il->begin(); j != il->end(); j++)
                {
                    _edgeDrawSet.insert(*j);
                }
            }
        }

        for(auto i = n->outputs().begin(); i != n->outputs().end(); i++)
        {
            NODEFLOW::ItemListPtr &il = *i;
            if(il)
            {
                for(auto j = il->begin(); j != il->end(); j++)
                {
                    _edgeDrawSet.insert(*j);
                }
            }
        }
    }
}

/*!
 * \brief CanvasWindows::drawNode
 */
void CanvasWindows::drawNode(wxDC &dc, NODEFLOW::NodePtr &n)
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
    getNodeEdges(n); // set of edges to draw
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
 * \brief CanvasWindows::drawEdge
 */
void CanvasWindows::drawEdge(wxDC &dc,NODEFLOW::EdgePtr &e)
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
 * \brief CanvasWindows::OnDraw
 * \param dc
 */
void CanvasWindows::OnDraw (wxDC &dc)
{
    _edgeDrawSet.clear();
    // draw the nodes
    for(auto i = _nodes.nodes().begin(); i != _nodes.nodes().end(); i++)
    {
        NODEFLOW::NodePtr &n = i->second;
        if(n) drawNode(dc,n);
    }
    //
    // now draw the edges
    //
    for(auto i = _edgeDrawSet.begin(); i != _edgeDrawSet.end(); i++)
    {
        NODEFLOW::EdgePtr &e = _nodes.findEdge(*i);
        if(e) drawEdge(dc,e);
    }
}


/*!
 * \brief CanvasWindows::onLeftDown
 * \param event
 */
void CanvasWindows::onLeftDown(wxMouseEvent &event)
{
    if(editMode())
    {
        if(_state ==IDLE)
        {
            int x,y,xx,yy ;
            event.GetPosition(&x,&y);
            CalcUnscrolledPosition( x, y, &xx, &yy );
            //
            _currentpoint = wxPoint( xx, yy ) ;
            _nodes.clearSelected(); // clear all selections
            //
            for(auto i = _nodes.nodes().begin(); i != _nodes.nodes().end(); i++)
            {
                NODEFLOW::NodePtr &n = i->second;
                //
                NODEFLOW::NodeType *t = NODEFLOW::NodeType::find(n->type());
                _currentLayout = t->nodeLayout(n->id());
                wxRect r = _currentLayout.rect();
                r.SetPosition(n->location());
                //
                if(r.Contains(_currentpoint))
                {
                    //
                    // Look for connection
                    _nodeStart = n->id();
                    //
                    int h = _currentLayout.hitInputConnector(_currentpoint,n->location());
                    if(h >= 0)
                    {
                        if(t->canConnectInput(n,h))
                        {
                            _state = INPUT_SELECT;
                            _startpoint = _currentpoint;
                            _connectorSelect = h;
                        }
                    }
                    else
                    {
                        h = _currentLayout.hitOutputConnector(_currentpoint,n->location());
                        if(h >= 0)
                        {
                            if(t->canConnectOutput(n,h))
                            {
                                _startpoint = _currentpoint;
                                _state = OUTPUT_SELECT;
                                _connectorSelect = h;
                            }
                        }
                    }
                    if( h < 0)
                    {
                        _startpoint = _currentpoint;
                        _state = NODE_SELECT;
                        _connectorSelect = -1;
                    }
                    //

                    _rect = r;
                    n->setSelected(true);
                    _currentNode = n.get();
                    Refresh();
                    CaptureMouse() ;
                    return;
                }
            }
            // now look for connections - a little harder
        }
    }
}

void CanvasWindows::onLeftUp(wxMouseEvent &event)
{
    if(editMode())
    {
    if ( _state != IDLE)
    {
        int x,y,xx,yy ;
        event.GetPosition(&x,&y);
        CalcUnscrolledPosition( x, y, &xx, &yy );
        //
        _currentpoint = wxPoint( xx, yy ) ;
        //
        ReleaseMouse();
        {
            wxClientDC dc( this );
            PrepareDC( dc );
            wxDCOverlay overlaydc( _overlay, &dc );
            overlaydc.Clear();
        }
        //
        _overlay.Reset();
        //
        switch(_state)
        {


        case NODE_SELECT:
            //
            if(_currentNode)
            {
                wxPoint p = _currentpoint - _startpoint;
                if((abs(p.x) > CONNECTION_SIZE ) && (abs(p.y)  > CONNECTION_SIZE  ))
                {
                    _currentNode->setLocation(_currentpoint);
                }
            }
            break;
        case INPUT_SELECT:
        {
            // are we over an output  - if so connect
            for(auto i = _nodes.nodes().begin(); i != _nodes.nodes().end(); i++)
            {
                NODEFLOW::NodePtr &n = i->second;
                //
                NODEFLOW::NodeType *t = NODEFLOW::NodeType::find(n->type());
                wxRect r = t->nodeLayout(n->id()).rect();
                r.SetPosition(n->location());
                //
                if(r.Contains(_currentpoint))
                {
                    //
                    // Look for connection
                    NODEFLOW::NodeLayout _targetLayout = n->layout();
                    //
                    int h = _targetLayout.hitOutputConnector(_currentpoint,n->location());
                    if(h >= 0)
                    {
                        //  found an output
                        // create an edge if allowed
                        _nodes.addConnect(n->id(),h,_nodeStart,_connectorSelect);
                    }
                    break;
                }
            }
        }
        break;
        case OUTPUT_SELECT:
        {
            // are we over an input if so connect
            for(auto i = _nodes.nodes().begin(); i != _nodes.nodes().end(); i++)
            {
                NODEFLOW::NodePtr &n = i->second;
                //
                NODEFLOW::NodeType *t = NODEFLOW::NodeType::find(n->type());
                wxRect r = t->nodeLayout(n->id()).rect();
                r.SetPosition(n->location());
                //
                NODEFLOW::NodeLayout _targetLayout = n->layout();
                //
                if(r.Contains(_currentpoint))
                {
                    //
                    // Look for connection
                    int h = _targetLayout.hitInputConnector(_currentpoint,n->location());
                    if(h >= 0)
                    {
                        //  found an output
                        // create an edge
                        _nodes.addConnect(_nodeStart,_connectorSelect,n->id(),h);
                    }
                    break;
                }
            }
        }
        break;
        default:
            break;
        }
        //
        _state = IDLE;
    }
    Refresh();
    Update();
    }
}



/*!
 * \brief CanvasWindows::onRightDoubleClick
 * \param event
 */
void CanvasWindows::onRightDoubleClick(wxMouseEvent& event)
{
}

/*!
 * \brief CanvasWindows::OnPopupClick
 * \param evt
 */
void CanvasWindows::onPopupClick(wxCommandEvent &evt)
{
    switch (evt.GetId()) {
    case wxID_EDIT:
        // Open properties for the node
        if(_currentNode)
        {
            _currentNode->save(_nodes); // save curent location
            _currentNode->nodeType()->properties(this,_nodes,_currentNode->id());
            _currentNode->load(_nodes); // load any changed attiributes
            Refresh();
        }

        break;
    case wxID_DELETE:
        // delete the node
        switch(_state)
        {
        case NODE_SELECT:
            if(_currentNode)
            {
                _nodes.deleteNode(_currentNode->id());
                _currentNode = nullptr;
            }
            break;
        case INPUT_SELECT:
            // delete edges
            if(_currentNode)
            {
                if(_current_connector >= 0)
                {
                    NODEFLOW::ItemListPtr &l = _currentNode->inputs()[_current_connector];
                    if(l)
                    {
                        NODEFLOW::ItemList c;
                        c = *l; // copy list
                        for(auto i = c.begin(); i != c.end(); i++ )
                        {
                            _nodes.disconnect(*i); // delete the edges - disconnect modifies the input lists on the nodes
                        }
                    }
                }
            }
            break;
        case OUTPUT_SELECT:
            if(_currentNode)
            {
                if(_current_connector >= 0)
                {
                    NODEFLOW::ItemListPtr &l = _currentNode->outputs()[_current_connector];
                    if(l)
                    {
                        NODEFLOW::ItemList c;
                        c = *l; // copy list
                        for(auto i = c.begin(); i != c.end(); i++ )
                        {
                            _nodes.disconnect(*i); // delete the edges - disconnect modifies the input lists on the nodes
                        }
                    }
                }
            }
            break;
        default:
            break;
        }
        //
        _currentNode = nullptr;
        _state = IDLE;
        //
        break;
    case wxID_CANCEL:
        break;
    default:
        break;
    }
}
/*!
 * \brief CanvasWindows::onRightDown
 * \param event
 */
void CanvasWindows::onRightDown(wxMouseEvent& event)
{
    // context menu
    int x,y,xx,yy ;
    event.GetPosition(&x,&y);
    CalcUnscrolledPosition( x, y, &xx, &yy );
    _currentpoint = wxPoint( xx, yy ) ;
    //
    for(auto i = _nodes.nodes().begin(); i != _nodes.nodes().end(); i++)
    {
        NODEFLOW::NodePtr &n = i->second;
        //
        NODEFLOW::NodeType *t = NODEFLOW::NodeType::find(n->type());
        _currentLayout = t->nodeLayout(n->id());
        wxRect r = _currentLayout.rect();
        r.SetPosition(n->location());
        _currentNode = n.get();
        wxMenu mnu;
        mnu.Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(CanvasWindows::onPopupClick), NULL, this);

        //
        if(r.Contains(_currentpoint))
        {
            if(editMode())
            {
                NODEFLOW::NodeLayout _targetLayout = n->layout();
                _current_connector = _targetLayout.hitInputConnector(_currentpoint,n->location());
                if(_current_connector >= 0)
                {
                    mnu.Append(wxID_DELETE,"Disconnect Inputs");
                    mnu.Append(wxID_CANCEL,"Cancel");
                    _state = INPUT_SELECT;
                }
                else
                {
                    _current_connector = _targetLayout.hitOutputConnector(_currentpoint,n->location());
                    if(_current_connector >= 0)
                    {
                        mnu.Append(wxID_DELETE,"Disconnect Outputs");
                        mnu.Append(wxID_CANCEL,"Cancel");
                        _state = OUTPUT_SELECT;
                    }
                    else
                    {
                        mnu.Append(wxID_EDIT,"Edit ...");
                        mnu.Append(wxID_DELETE,"Delete");
                        mnu.Append(wxID_CANCEL,"Cancel");
                        _state = NODE_SELECT;
                    }
                }
            }
            else
            {
                mnu.Append(wxID_EDIT,"Edit ...");
                mnu.Append(wxID_CANCEL,"Cancel");
                _state = NODE_SELECT;
            }
            PopupMenu(&mnu);
            _state = IDLE;
            Refresh();
            return;
        }
    }
}
/*!
 * \brief CanvasWindows::onRightUp
 * \param event
 */
void CanvasWindows::onRightUp(wxMouseEvent& event)
{
}
/*!
 * \brief CanvasWindows::onWheel
 * \param event
 */
void CanvasWindows::onWheel(wxMouseEvent& event)
{
}


