#include "nodedraw.h"
#include "nodeset.h"
#include <set>
/*!
 * \brief NODEFLOW::NodeDraw::draw
 * \param s
 * \param rect
 */
void NODEFLOW::NodeDraw::draw(NodeSet &ns , wxRect &rect)
{
    _edgeDrawSet.clear();
   begin();
   // draw nodes
   for(auto i = ns.nodes().begin(); i != ns.nodes().end(); i++)
   {
        NodePtr &n = i->second;
        if(n)
        {
            NodeType *t = NodeType::find(n->type());
            wxRect r = t->nodeLayout(n->id()).rect();
            r.SetPosition(n->location());
            //
            if(rect.Intersects(r))
            {
                //
                drawNode(ns,n);
                //
            }
        }
   }
   //
   // draw edges as required
   //
   std::for_each(_edgeDrawSet.cbegin(), _edgeDrawSet.cend(), [&](unsigned x) {
      EdgePtr &e = ns.findEdge(x);
      if(e)drawEdge(ns,e);
    });
   end();
}

/*!
 * \brief NODEFLOW::NodeDraw::getNodeEdges
 * \param n
 */
void NODEFLOW::NodeDraw::getNodeEdges(NodePtr &n)
{
   if(n)
   {
       // draw the connected edges
       for(auto i = n->inputs().begin(); i != n->inputs().end(); i++)
       {
           ItemListPtr &il = *i;
           for(auto j = il->begin(); j != il->end(); j++)
           {
               _edgeDrawSet.insert(*j);
           }
       }

       for(auto i = n->outputs().begin(); i != n->outputs().end(); i++)
       {
           ItemListPtr &il = *i;
           for(auto j = il->begin(); j != il->end(); j++)
           {
               _edgeDrawSet.insert(*j);
           }
       }
   }
}

/*!
 * \brief NODEFLOW::NodeDraw::drawNode
 */
void NODEFLOW::NodeDraw::drawNode(NodeSet &ns, NodePtr &n, bool selected)
{
    NodeType *t = NodeType::find(n->type());
    wxRect r = t->nodeLayout(n->id()).rect();
    r.SetPosition(n->location());

    // draw a rect angle
    // draw input connections
    // draw output connections
    // draw the title
    setPen(PenForeground);
    getNodeEdges(n);
}

/*!
 * \brief NODEFLOW::NodeDraw::drawEdge
 */
void NODEFLOW::NodeDraw::drawEdge(NodeSet &ns,EdgePtr &e)
{
    // draw a bezier between the ends
    //
    // Get the nodes
    // Get the layouts
    // get the connection positions
    //
    //draw it
}

/*!
 * \brief NODEFLOW::NodeDraw::eraseNode
 */
void NODEFLOW::NodeDraw::eraseNode(NodeSet &ns,NodePtr &n)
{
    NodeType *t = NodeType::find(n->type());
    wxRect r = t->nodeLayout(n->id()).rect();
    r.SetPosition(n->location());
    // now fill to erase
    setPen(PenBackground);
    // now erase the connected edges
    // draw the connected edges
    for(auto i = n->inputs().begin(); i != n->inputs().end(); i++)
    {
        ItemListPtr &il = *i;
        for(auto j = il->begin(); j != il->end(); j++)
        {
            EdgePtr &e = ns.findEdge(*j);
            drawEdge(ns,e);
        }
    }

    for(auto i = n->outputs().begin(); i != n->outputs().end(); i++)
    {
        ItemListPtr &il = *i;
        for(auto j = il->begin(); j != il->end(); j++)
        {
            EdgePtr &e = ns.findEdge(*j);
            drawEdge(ns,e);
        }
    }

}




#if 0
NODEFLOW::NodeDraw::NodeDraw () :
    width (0),
    height (0),
    memoryBitmap (new wxBitmap (wxSize (1, 1))),
    memoryDC (new wxMemoryDC ())
{
    memoryDC->SelectObject (*memoryBitmap);
    graphicsContext = wxGraphicsContext::Create (*memoryDC);
}

NODEFLOW::NodeDraw::~NodeDraw ()
{
    delete memoryBitmap;
    delete memoryDC;
    delete graphicsContext;
}

void NODEFLOW::NodeDraw::DrawToDC (wxDC* targetDC)
{
    try
    {
        targetDC->Blit (0, 0, width, height, memoryDC, 0, 0);
    }
    CATCH_DEF
}

void NODEFLOW::NodeDraw::Init (void*)
{

}

void NODEFLOW::NodeDraw::BlitToWindow (wxWindow *nativeHandle)
{
    try
    {
        wxPaintDC dc (nativeHandle);
        BlitToContext (&dc);
    }
    CATCH_DEF
}

void NODEFLOW::NodeDraw::BlitToContext (wxPaintDC* dc)
{
    try
    {
        dc->Blit (0, 0, width, height, memoryDC, 0, 0);
    }
    CATCH_DEF
}

void NODEFLOW::NodeDraw::Resize (int newWidth, int newHeight)
{
    try
    {
        width = newWidth;
        height = newHeight;

        delete memoryBitmap;
        delete memoryDC;
        delete graphicsContext;

        memoryBitmap = new wxBitmap (newWidth, newHeight);
        memoryDC = new wxMemoryDC ();
        memoryDC->SelectObject (*memoryBitmap);
        graphicsContext = wxGraphicsContext::Create (*memoryDC);
    }
    CATCH_DEF
}

int NODEFLOW::NodeDraw::GetWidth () const
{
    return width;
}

int NODEFLOW::NodeDraw::GetHeight () const
{
    return height;
}

void NODEFLOW::NodeDraw::BeginDraw ()
{

}

void NODEFLOW::NodeDraw::EndDraw ()
{

}


void NODEFLOW::NodeDraw::DrawLine (const wxPoint beg, const wxPoint end, const wxPen& pen)
{
    try
    {
        graphicsContext->SetBrush (*wxTRANSPARENT_BRUSH);
        graphicsContext->SetPen (pen);
        std::vector<wxPoint2DDouble> lines;
        lines.push_back (wxPoint2DDouble (beg.x (), beg.y ()));
        lines.push_back (wxPoint2DDouble (end.x (), end.y ()));
        graphicsContext->DrawLines (lines.size (), lines.data ());
    }
    CATCH_DEF
}

void NODEFLOW::NodeDraw::DrawBezier (const wxPoint p1, const wxPoint p2, const wxPoint p3, const wxPoint p4, const wxPen& pen)
{
    try
    {
        graphicsContext->SetBrush (*wxTRANSPARENT_BRUSH);
        graphicsContext->SetPen (pen);
        wxGraphicsPath path = graphicsContext->CreatePath ();
        path.MoveToPoint (p1);
        path.AddCurveToPoint (p2.x, p2.y, p3.x, p3.y, p4.x, p4.y);
        graphicsContext->DrawPath (path);
    }
    CATCH_DEF
}

void NODEFLOW::NodeDraw::DrawRect (const wxRect& rect, const wxPen& pen)
{
    try
    {
        graphicsContext->SetBrush (*wxTRANSPARENT_BRUSH);
        graphicsContext->SetPen (pen);
        graphicsContext->DrawRectangle (rect.GetLeft(), rect.GetTop(), rect.GetWidth (), rect.GetHeight ());
    }
    CATCH_DEF
}

void NODEFLOW::NodeDraw::FillRect (const wxRect& rect, const wxColour& color)
{
    try
    {
        graphicsContext->SetBrush (wxBrush (color));
        graphicsContext->SetPen (*wxTRANSPARENT_PEN);
        graphicsContext->DrawRectangle (rect.GetLeft(), rect.GetTop (), rect.GetWidth (), rect.GetHeight ());
    }
    CATCH_DEF
}

void NODEFLOW::NodeDraw::DrawEllipse (const wxRect& rect, const wxPen& pen)
{
    try
    {
        graphicsContext->SetBrush (*wxTRANSPARENT_BRUSH);
        graphicsContext->SetPen (pen);
        graphicsContext->DrawEllipse (rect.GetLeft(), rect.GetTop (), rect.GetWidth (), rect.GetHeight ());
    }
    CATCH_DEF
}

void NODEFLOW::NodeDraw::FillEllipse (const wxRect& rect, const wxColour& color)
{
    try
    {
        graphicsContext->SetBrush (wxBrush (color));
        graphicsContext->SetPen (*wxTRANSPARENT_PEN);
        graphicsContext->DrawEllipse (rect.GetLeft (), rect.GetTop(), rect.GetWidth (), rect.GetHeight ());
    }
    CATCH_DEF
}

void NODEFLOW::NodeDraw::DrawFormattedText (const wxRect& rect, const wxFont& font, const std::wstring& text, int hAnchor, int vAnchor, const wxColour& textColor)
{
    try
    {
        memoryDC->SetTextForeground (textColor);
        memoryDC->SetFont (font);
        int alignment = 0;
        memoryDC->DrawLabel (text, rect, alignment);
    }
    CATCH_DEF
}

wxSize  NODEFLOW::NodeDraw::MeasureText (const wxFont& font, const std::wstring& text)
{
    try
    {
        memoryDC->SetFont (font);
        wxSize size = memoryDC->GetTextExtent (text);
        return wxSize (size.x, size.y);
    }
    CATCH_DEF
}

bool NODEFLOW::NodeDraw::CanDrawIcon ()
{
    return false;
}

void NODEFLOW::NodeDraw::DrawIcon (const wxRect&, const wxIcon&)
{
    DBGBREAK ();
}

#endif
