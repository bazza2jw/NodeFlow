/*
 * Copyright (C) 2022 -  B. J. Hill
 *
 * This file is part of OpcServant. OpcServant C++ classes are free software: you can
 * redistribute it and/or modify it under the terms of the Mozilla Public
 * License v2.0 as stated in the LICENSE file .
 *
 * These classes are distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.
 */

#ifndef NODEDRAW_H
#define NODEDRAW_H
#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#   include "wx/wx.h"
#endif
//
#include <memory>
#include <map>
#include <wx/gdicmn.h>
#include <wx/dcmemory.h>
#include <wx/graphics.h>
#include "nodetype.h"
#include "edge.h"
#include <Wt/WPaintDevice.h>
#include <Wt/WRectF.h>
#include <Wt/WPoint.h>
#include <Wt/WPointF.h>
#include <Wt/WPen.h>
#include <Wt/WBrush.h>
#include <Wt/WColor.h>
#include <Wt/WFont.h>
#include <Wt/WPaintedWidget.h>
#include <Wt/WPainter.h>
#include <Wt/WPainterPath.h>

// This resolves platform differences - there is a lot of same but different code
// a lot of things are the same but use different functions calls and formats

namespace NODEFLOW
{

class NodeSet;

/*!
 * \brief The NodeDraw class
 * can draw to screen or bitmap
 * encapsulate the drawing of a node set
 */
class NodeDrawWx
{
    EdgeIdSet _edgeDrawSet; // only draw an edge once if necesary
    wxPen _normalPen;
    wxPen _selectPen;
    wxPen _erasePen;
    wxBrush _eraseBrush;
    //
    wxFont _connectionFont;
    wxFont _titleFont;
    NodeSet &_nodeSet;
    virtual void drawNode(wxDC &dc, NodePtr &);
    virtual void drawEdge(wxDC &dc, EdgePtr &);
public:
    NodeDrawWx (NodeSet &s );
    NodeDrawWx (const NodeDrawWx & rhs) = delete;
    virtual ~NodeDrawWx () {}
    virtual void draw(wxDC &dc);
    void drawSpline(wxDC &dc,wxPoint beg, wxPoint end)
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
        dc.DrawSpline(4,pts);
    }
};

//
// The Wt version
//
class NodeDrawWt
{
    EdgeIdSet _edgeDrawSet; // only draw an edge once if necesary
    //
    Wt::WPen _normalPen;
    Wt::WPen _selectPen;
    Wt::WPen _erasePen;
    Wt::WBrush _eraseBrush;
    Wt::WFont _connectionFont;
    Wt::WFont _titleFont;
    Wt::WBrush _boolBrush;
    Wt::WBrush _intBrush;
    Wt::WBrush _floatBrush;
    Wt::WBrush _stringBrush;
    Wt::WBrush _anyBrush;
    //
    NodeSet &_nodeSet;
    //
    virtual void drawNode(Wt::WPainter &dc, NodePtr &n);
    virtual void drawEdge(Wt::WPainter &dc, EdgePtr &);
    //
public:
    void drawSpline(Wt::WPainter &dc,wxPoint beg, wxPoint end);
    NodeDrawWt (NodeSet &s );
    NodeDrawWt (const NodeDrawWt & rhs) = delete;
    virtual ~NodeDrawWt () {}
    virtual void draw(Wt::WPainter &dc);
};




}
#endif // NODEDRAW_H
