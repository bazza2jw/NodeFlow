#ifndef WEBCANVAS_H
#define WEBCANVAS_H
#include <Wt/WPaintedWidget.h>
#include "NodeFlow/nodeset.h"
#include <Wt/WPoint.h>
#include <Wt/WRectArea.h>
#include "NodeFlow/nodedraw.h"
#include <Wt/WCssDecorationStyle.h>
#include <Wt/WColor.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WEvent.h>
#include <Wt/WPainter.h>
#include <Wt/WPaintedWidget.h>
#include <Wt/WPainterPath.h>
#include <Wt/WPen.h>
#include <Wt/WPointF.h>
#include <Wt/WPushButton.h>
#include <Wt/WRectF.h>
#include <Wt/WTemplate.h>
#include <Wt/WText.h>
#include <Wt/WToolBar.h>
namespace NODEFLOW
{

    class WebCanvas : public Wt::WPaintedWidget
    {

        int _width = 2048;
        int _height = 2048;
        int _scrollInterval = 10;
        int _nextId = 1000;
        wxRect _canvasSize;
        //

        int _state = 0;
        Wt::WPointF _currentpoint;
        Wt::WPointF _startpoint;
        //
        std::string _fileName;
        //
        NODEFLOW::NodeSet _nodes;
        //
        bool _editMode = true;
        //
        NodeSet::HitStruct _startHit; // start of drag
        NodeSet::HitStruct _currentHit; // current / end of drag
        //
        Wt::WImage _baseLayer; // the design render to a bitmap
        //
        void drawSpline(Wt::WPainter &dc,wxPoint beg, wxPoint end);
        //
        void redraw(int i = 0)
        {
            update();
        }
        // Drawing routines
        EdgeIdSet _edgeDrawSet; // only draw an edge once if necesary
        Wt::WPen _normalPen;
        Wt::WPen _selectPen;
        //
        Wt::WBrush _boolBrush;
        Wt::WBrush _intBrush;
        Wt::WBrush _floatBrush;
        Wt::WBrush _stringBrush;
        Wt::WBrush _anyBrush;
        //
        Wt::WFont _connectionFont;
        Wt::WFont _titleFont;
        //

        //
        virtual void drawNode(Wt::WPainter &dc, NodePtr &n);
        virtual void drawEdge(Wt::WPainter &dc, EdgePtr &e);
        virtual void draw(Wt::WPainter &dc, const wxRect &area);

    public:
        WebCanvas();
        ~WebCanvas();
        bool editMode() const { return _editMode; }
        virtual void paintEvent (Wt::WPaintDevice *paintDevice);
        void mouseDown(const Wt::WMouseEvent& e);
        void mouseDrag(const Wt::WMouseEvent& e);
        void mouseUp(const Wt::WMouseEvent& e);
        //
        NODEFLOW::NodeSet & nodes() { return  _nodes;}
        //
        void readSet(const std::string &s);
        void save();
        void saveAs(const std::string &s);
    };
}
#endif // WEBCANVAS_H
