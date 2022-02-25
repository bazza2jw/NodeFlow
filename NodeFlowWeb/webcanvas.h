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
        NodeDrawWt _draw;
        Wt::WPainterPath path_;
        Wt::WColor color_;
        //
        NodeSet::HitStruct _startHit; // start of drag
        NodeSet::HitStruct _currentHit; // current / end of drag
        //
    public:
        WebCanvas();
        ~WebCanvas();
        bool editMode() const { return _editMode; }
        virtual void paintEvent (Wt::WPaintDevice *paintDevice);

        void mouseDown(const Wt::WMouseEvent& e);
        void mouseDrag(const Wt::WMouseEvent& e);
        void mouseUp(const Wt::WMouseEvent& e);


    };
}
#endif // WEBCANVAS_H
