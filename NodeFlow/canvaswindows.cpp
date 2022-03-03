/*
 * Copyright (C) 2022 -  B. J. Hill
 *
 * This file is part of NodeFlow. NodeFlow C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */

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
    _connectionFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL),
    _titleFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL),
    _draw(_nodes)

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
    Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(CanvasWindows::onRightDown), NULL, this);
    Connect(wxEVT_CHAR, wxKeyEventHandler(CanvasWindows::onChar), NULL, this);
    Connect(wxEVT_MOTION, wxMouseEventHandler(CanvasWindows::onMotion), NULL, this);
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
    _state = NODEFLOW::NodeSet::NONE;
    ReleaseMouse();
    Refresh();
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
        // TBD
    }
    break;

    default:
        break;
    }
}


void CanvasWindows::onMotion(wxMouseEvent &event)
{
    if(editMode())
    {
        if ( _state != NODEFLOW::NodeSet::NONE)
        {
            int x,y, xx, yy ;
            event.GetPosition(&x,&y);
            CalcUnscrolledPosition( x, y, &xx, &yy );
            _currentpoint = wxPoint( xx, yy ) ;
            wxClientDC dc( this ) ;
            PrepareDC( dc ) ;
            wxDCOverlay overlaydc( _overlay, &dc );
            overlaydc.Clear();
            dc.SetPen( wxPen( *wxLIGHT_GREY, 2 ) );
            dc.SetBrush( *wxTRANSPARENT_BRUSH );
            switch(_state)
            {
            case NODEFLOW::NodeSet::INPUT_SELECT:
                _draw.drawSpline(dc,_startpoint,_currentpoint);
                break;
            case NODEFLOW::NodeSet::OUTPUT_SELECT:
                _draw.drawSpline(dc,_startpoint,_currentpoint);
                break;
            case NODEFLOW::NodeSet::NODE_SELECT:
                _rect = _startHit._currentRect;
                _rect.SetLeftTop(_currentpoint); // translate
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
 * \brief CanvasWindows::OnDraw
 * \param dc
 */
void CanvasWindows::OnDraw (wxDC &dc)
{
    wxRect r = this->GetRect(); // when pan and zoom added change to draw only in region
    _draw.draw(dc,r);
}


/*!
 * \brief CanvasWindows::onLeftDown
 * \param event
 */
void CanvasWindows::onLeftDown(wxMouseEvent &event)
{
    if(editMode())
    {
        if(_state ==NODEFLOW::NodeSet::NONE)
        {
            int x,y,xx,yy ;
            event.GetPosition(&x,&y);
            CalcUnscrolledPosition( x, y, &xx, &yy );
            _currentpoint = wxPoint( xx, yy ) ;
            _nodes.clearSelected(); // clear all selections
            _startHit.clear();
            _state = _nodes.hitTest(_currentpoint,_startHit);
            if(_state != NODEFLOW::NodeSet::NONE)
            {
                _startpoint = _currentpoint;
                _currentHit = _startHit;
                Refresh();
                CaptureMouse() ;
            }
        }
    }
}

void CanvasWindows::onLeftUp(wxMouseEvent &event)
{
    if(editMode())
    {
        if ( _state != NODEFLOW::NodeSet::NONE)
        {

            ReleaseMouse();
            {
                wxClientDC dc( this );
                PrepareDC( dc );
                wxDCOverlay overlaydc( _overlay, &dc );
                overlaydc.Clear();
            }
            _overlay.Reset();

            int x,y,xx,yy ;
            event.GetPosition(&x,&y);
            CalcUnscrolledPosition( x, y, &xx, &yy );
            //
            _currentpoint = wxPoint( xx, yy );

            switch(_state)
            {
            case NODEFLOW::NodeSet::INPUT_SELECT:
            case NODEFLOW::NodeSet::OUTPUT_SELECT:
                _nodes.makeConnectionSelect(_currentpoint, _startpoint, _state, _startHit);
                break;
            case NODEFLOW::NodeSet::NODE_SELECT:
            {
                _startHit._node->setLocation(_currentpoint);
            }
            break;
            default:
                break;
            }
            Refresh();
            Update();
        }
    }
    _state = NODEFLOW::NodeSet::NONE;
}



/*!
 * \brief CanvasWindows::OnPopupClick
 * \param evt
 */
void CanvasWindows::onPopupClick(wxCommandEvent &evt)
{
    if(_currentHit._node)
    {
        switch (evt.GetId()) {
        case wxID_EDIT:
            // Open properties for the node
            _currentHit._node->save(_nodes); // save curent location
            _currentHit._node->nodeType()->properties(this,_nodes,_currentHit._nodeId);
            _currentHit._node->load(_nodes); // load any changed attiributes
            Refresh();

            break;
        case wxID_DELETE:
            // delete the node
            switch(_state)
            {
            case NODEFLOW::NodeSet::NODE_SELECT:
                _nodes.deleteNode(_currentHit._nodeId);
                break;
            case NODEFLOW::NodeSet::INPUT_SELECT:
                // delete edges
                _nodes.removeEdgesFromInput(_currentHit._nodeId,_currentHit._connectorSelect);

                break;
            case NODEFLOW::NodeSet::OUTPUT_SELECT:
                _nodes.removeEdgesFromOutput(_currentHit._nodeId,_currentHit._connectorSelect);
                break;
            default:
                break;
            }
            //
            break;
        case wxID_CANCEL:
            break;
        default:
            break;
        }
    }
}
/*!
 * \brief CanvasWindows::onRightDown
 * \param event
 */
void CanvasWindows::onRightDown(wxMouseEvent& event)
{
    if(editMode())
    {
        // context menu
        int x,y,xx,yy ;
        event.GetPosition(&x,&y);
        CalcUnscrolledPosition( x, y, &xx, &yy );
        _currentpoint = wxPoint( xx, yy ) ;
        _state = _nodes.hitTest(_currentpoint,_startHit);
        _currentHit = _startHit;
        //
        if(_currentHit._node)
        {
            wxMenu mnu;
            mnu.Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(CanvasWindows::onPopupClick), NULL, this);
            //
            switch(_state)
            {
            case NODEFLOW::NodeSet::NODE_SELECT:
                mnu.Append(wxID_EDIT,"Edit ...");
                mnu.Append(wxID_DELETE,"Delete");
                mnu.Append(wxID_CANCEL,"Cancel");
                break;
            case NODEFLOW::NodeSet::INPUT_SELECT:
                mnu.Append(wxID_DELETE,"Disconnect Inputs");
                mnu.Append(wxID_CANCEL,"Cancel");
                break;
            case NODEFLOW::NodeSet::OUTPUT_SELECT:
                mnu.Append(wxID_DELETE,"Disconnect Outputs");
                mnu.Append(wxID_CANCEL,"Cancel");
                break;

            default:
                break;
            }
            PopupMenu(&mnu);
        }
    }
    _state = NODEFLOW::NodeSet::NONE;
    Refresh();
}
