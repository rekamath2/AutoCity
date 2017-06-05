#include <iostream>
#include "MwxGLCanvas.h"

using namespace std;

const long MwxGLCanvas::ID_GLCANVAS = wxNewId();

MwxGLCanvas::MwxGLCanvas(wxWindow* parent, int* attribList, wxSize sizeXY)
             : wxGLCanvas(parent,
                          ID_GLCANVAS,
                          attribList,
                          wxDefaultPosition,
                          sizeXY,
                          wxFULL_REPAINT_ON_RESIZE)
{
    // ctor
    //Connect(wxEVT_PAINT,
    //  (wxObjectEventFunction)&EwxGLCanvas::OnGLCanvasPaint);
    //Connect(wxEVT_KEY_DOWN,
    //  (wxObjectEventFunction)&EwxGLCanvas::OnGLCanvasKeyDown);
    //Connect(wxEVT_KEY_UP,
    //  (wxObjectEventFunction)&EwxGLCanvas::OnGLCanvasKeyUp);
    Connect(wxEVT_LEFT_DOWN,
      (wxObjectEventFunction)&MwxGLCanvas::OnGLCanvasLeftMouseButtonDown);
    Connect(wxEVT_LEFT_UP,
      (wxObjectEventFunction)&MwxGLCanvas::OnGLCanvasLeftMouseButtonUp);
    Connect(wxEVT_RIGHT_DOWN,
      (wxObjectEventFunction)&MwxGLCanvas::OnGLCanvasRightMouseButtonDown);
    Connect(wxEVT_RIGHT_UP,
      (wxObjectEventFunction)&MwxGLCanvas::OnGLCanvasRightMouseButtonUp);
    Connect(wxEVT_MOTION,
      (wxObjectEventFunction)&MwxGLCanvas::OnGLCanvasMouseMove);
    Connect(wxEVT_MOUSEWHEEL,
      (wxObjectEventFunction)&MwxGLCanvas::OnGLCanvasMouseWheel);
}

MwxGLCanvas::~MwxGLCanvas()
{
    //dtor
}

/* If not using Timer event, just use Paint event and Refresh function
void MwxGLCanvas::OnGLCanvasPaint(wxPaintEvent& event)
{
}
*/
/*
void MwxGLCanvas::OnGLCanvasKeyDown(wxKeyEvent& event)
{
    cout << "OnGLCanvasKeyDown() is triggered !!" << endl;
}

void MwxGLCanvas::OnGLCanvasKeyUp(wxKeyEvent& event)
{
    cout << "OnGLCanvasKeyUp() is triggered !!" << endl;
}
*/
void MwxGLCanvas::OnGLCanvasLeftMouseButtonDown(wxMouseEvent& event)
{
    //cout << "OnGLCanvasLeftMouseButtonDown() is triggered !!" << endl;
}

void MwxGLCanvas::OnGLCanvasLeftMouseButtonUp(wxMouseEvent& event)
{
    //cout << "OnGLCanvasLeftMouseButtonUp() is triggered !!" << endl;
}

void MwxGLCanvas::OnGLCanvasRightMouseButtonDown(wxMouseEvent& event)
{
    //cout << "OnGLCanvasRightMouseButtonDown() is triggered !!" << endl;
}

void MwxGLCanvas::OnGLCanvasRightMouseButtonUp(wxMouseEvent& event)
{
    //cout << "OnGLCanvasRightMouseButtonUp() is triggered !!" << endl;
}

void MwxGLCanvas::OnGLCanvasMouseMove(wxMouseEvent& event)
{
    //cout << "OnGLCanvasMouseMove() is triggered !!" << endl;
}

void MwxGLCanvas::OnGLCanvasMouseWheel(wxMouseEvent& event)
{
    //cout << "OnGLCanvasMouseWheel() is triggered !!" << endl;
}
