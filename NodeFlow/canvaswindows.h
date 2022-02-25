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

#ifndef CANVASWINDOWS_H
#define CANVASWINDOWS_H
#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#   include "wx/wx.h"
#endif
#include <wx/overlay.h>
#include <wx/scrolwin.h>
#include <wx/dnd.h>
#include "../NodeFlow/nodeset.h"
#include "../NodeFlow/nodedraw.h"


class CanvasWindows : public wxScrolled<wxWindow> {
    int _width = 2048;
    int _height = 2048;
    int _scrollInterval = 10;
    int _nextId = 1000;
    //
    int _state = 0;
    wxRect _rect;
    wxPoint _currentpoint;
    wxPoint _startpoint;
    //
    unsigned _nodeStart = 0;
    int selectMode = NODEFLOW::NodeSet::NONE;
    std::string _fileName;
    wxPoint lastPt;
    wxOverlay _overlay;
    //
    NODEFLOW::NodeSet::HitStruct _startHit; // start of drag
    NODEFLOW::NodeSet::HitStruct _currentHit; // current / end of drag
    //
    void addNode(int x, int y,const std::string &s);
    void onPopupClick(wxCommandEvent &evt);
    //
    class CanvasDropTarget : public wxTextDropTarget
    {
        CanvasWindows * _parent = nullptr;
    public:
        CanvasDropTarget(CanvasWindows *p) : _parent(p) {}
        virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& data)
        {
            _parent->addNode(x,y,data.ToStdString());
            return true;
        }
    };
    //
    CanvasDropTarget * _dropTarget = nullptr;
    //
    NODEFLOW::NodeSet _nodes; // the data
    // The pens to use
    //
    wxPen _normalPen;
    wxPen _selectPen;
    //
    wxFont _connectionFont;
    wxFont _titleFont;
    bool _editMode = true;

    NODEFLOW::NodeDrawWx _draw;
public:

    CanvasWindows(wxWindow *parent, int w = 2048, int h = 2048, int scrollInterval = 10);
    //
    void newFlow()
    {
        _nodes.clear();
        Refresh();
    }
    //
    bool editMode() const { return _editMode;}
    void setEditMode(bool f) { _editMode = f;}
    //
    void open();
    void readSet(const std::string &);
    void save();
    void saveAs();
    //
    // GUI based run (for testing flows)
    //
    NODEFLOW::NodeSet & nodeSet() { return _nodes;}

    virtual void OnDraw (wxDC &dc);
    virtual void OnCaptureLost(wxMouseCaptureLostEvent& event);
    virtual void onChar(wxKeyEvent& event);
    virtual void onLeftDown(wxMouseEvent& event);
    virtual void onLeftUp(wxMouseEvent& event);
    virtual void onMotion(wxMouseEvent& event);
    virtual void onRightDown(wxMouseEvent& event);

};

#endif // CANVASWINDOWS_H
