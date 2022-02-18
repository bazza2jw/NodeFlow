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



class CanvasWindows : public wxScrolled<wxWindow> {
    int _width = 2048;
    int _height = 2048;
    int _scrollInterval = 10;
    int _nextId = 1000;
    //
    enum
    {
        IDLE = 0,
        NODE_SELECT,
        INPUT_SELECT,
        OUTPUT_SELECT,
        SELECT_NODE,
        DRAW_NOODLE
    };
    int _state = 0;
    wxRect _rect;
    wxPoint _currentpoint;
    wxPoint _startpoint;
    //
    int _connectorSelect;
    unsigned _nodeStart = 0;
    int selectMode = IDLE;
    std::string _fileName;
    //
    wxPoint _anchorpoint;
    wxPoint lastPt;
    wxOverlay _overlay;

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
    //
    NODEFLOW::EdgeIdSet _edgeDrawSet; // only draw an edge once if necesary

    NODEFLOW::NodeLayout _currentLayout;
    NODEFLOW::Node * _currentNode = nullptr;
    int _current_connector = -1;
    //
    void getNodeEdges(NODEFLOW::NodePtr &);
    void drawNode(wxDC &, NODEFLOW::NodePtr &);
    void drawEdge(wxDC &, NODEFLOW::EdgePtr &);
    //
    // The pens to use
    //
    wxPen _normalPen;
    wxPen _selectPen;
    wxPen _erasePen;
    //
    wxBrush _eraseBrush;

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

public:

    CanvasWindows(wxWindow *parent, int w = 2048, int h = 2048, int scrollInterval = 10);
    //
    void newFlow()
    {
        _nodes.clear();
        Refresh();
    }

    void open();
    void save();
    void saveAs();
    //
    virtual void OnDraw (wxDC &dc);
    virtual void OnCaptureLost(wxMouseCaptureLostEvent& event);
    virtual void onChar(wxKeyEvent& event);
    virtual void onLeftDoubleClick(wxMouseEvent& event);
    virtual void onLeftDown(wxMouseEvent& event);
    virtual void onLeftUp(wxMouseEvent& event);
    virtual void onMotion(wxMouseEvent& event);
    virtual void onRightDoubleClick(wxMouseEvent& event);
    virtual void onRightDown(wxMouseEvent& event);
    virtual void onRightUp(wxMouseEvent& event);
    virtual void onWheel(wxMouseEvent& event);

};

#endif // CANVASWINDOWS_H
