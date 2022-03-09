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

#ifndef NODEDRAW_H
#define NODEDRAW_H
#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#   include "wx/wx.h"
#endif
//
#include "nodeflow.h"
#include <memory>
#include <map>
#include <wx/gdicmn.h>
#include <wx/dcmemory.h>
#include <wx/graphics.h>
#include "nodetype.h"
#include "edge.h"
#include <wx/url.h>
//

//
// This resolves platform differences - there is a lot of same but different code
// a lot of things are the same but use different functions calls and formats
// The Wt version has to be structured differently to the wxWidgets (or Qt)

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
    virtual void draw(wxDC &dc, const wxRect &area);
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

/*!
 * \brief The NodeDrawBitmap class
 * Draw to a bit map
 */
class NodeDrawBitmap : public NodeDrawWx
{
    wxSize                              _size;
    wxBitmap                            _bitmap;
    //
    static unsigned _gSession;
    unsigned _session = 100;
    wxString _bitFilename;
    wxString _url;
public:
    NodeDrawBitmap(NodeSet &s, int w, int h)
        : NodeDrawWx(s),
          _bitmap(2048,2048,32),
          _size(2048,2048)
    {
        _session = _gSession++;
    }
    /*!
     * \brief session
     */
    unsigned session() const { return _session;}
    //
    const wxString & bitFilename() { return _bitFilename;}
    //
    const wxString & url() const { return _url; }
    //
    void draw(wxRect &area)
    {
        wxMemoryDC DC(_bitmap);
        NodeDrawWx::draw(DC,area);
        DC.SelectObject(wxNullBitmap);
    }
    //
    const wxSize & size() { return  _size;}
    //
    void store()
    {
            _bitFilename = wxString::Format(NODEFLOW_ROOT_DIR "/bin/canvas/%08X.png",_session); // save for access in the bin directory which is the doc root
            _url = wxString::Format( "canvas/%08X.png",_session);
            _bitmap.SaveFile(_bitFilename,wxBITMAP_TYPE_PNG);
    }

};





}
#endif // NODEDRAW_H
