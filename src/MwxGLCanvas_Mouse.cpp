#include <iostream>
#include "MwxGLCanvas_Mouse.h"

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

    IsLMBPressed = false;
    dXmouse = dYmouse = dZmouse = 0;

}

MwxGLCanvas::~MwxGLCanvas()
{
    //dtor
}

void MwxGLCanvas::OnGLCanvasLeftMouseButtonDown(wxMouseEvent& event)
{
    //cout << "OnGLCanvasLeftMouseButtonDown() is triggered !!" << endl;
    IsLMBPressed = true;
    //m_mousePosOld = GetPosition();
    m_mousePosOld = event.GetPosition();
    dXmouse = dYmouse = 0;
}

void MwxGLCanvas::OnGLCanvasLeftMouseButtonUp(wxMouseEvent& event)
{
    //cout << "OnGLCanvasLeftMouseButtonUp() is triggered !!" << endl;
    IsLMBPressed = false;
    dXmouse = dYmouse = 0;
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
    if (IsLMBPressed)
    {
        m_mousePosNow = event.GetPosition();
        dXmouse += m_mousePosNow.x - m_mousePosOld.x;
        dYmouse += m_mousePosNow.y - m_mousePosOld.y;
        m_mousePosOld = m_mousePosNow;

        //cout << "x = " << m_mousePosNow.x  << ", y = " << m_mousePosNow.y << endl;
        //cout << "dx = " << dXmouse << ", dy = " << dYmouse << endl;
    }
}

void MwxGLCanvas::OnGLCanvasMouseWheel(wxMouseEvent& event)
{
    //cout << "OnGLCanvasMouseWheel() is triggered !!" << endl;
    if (!IsMWheelRotated)
    {
        dZmouse = 0;
        IsMWheelRotated = true;
    }
    dZmouse += event.GetWheelRotation() / -60;
    //cout << "dz = " << dZmouse << endl;
}
