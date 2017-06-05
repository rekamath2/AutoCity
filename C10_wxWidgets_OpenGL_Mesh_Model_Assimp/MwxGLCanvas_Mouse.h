#ifndef MWXGLCANVAS_MOUSE_H
#define MWXGLCANVAS_MOUSE_H

#include <GL/glew.h> // Must include glew.h ahead of glcanvas.h
#include <wx/glcanvas.h>

class MwxGLCanvas : public wxGLCanvas
{
    wxPoint m_mousePosNow, m_mousePosOld;

public:
    MwxGLCanvas(wxWindow* parent, int* attribList, wxSize sizeXY);
    ~MwxGLCanvas(void);

    bool IsLMBPressed, IsMWheelRotated;
    int  dXmouse, dYmouse, dZmouse;

protected:
    static const long ID_GLCANVAS;

private:
    // ---------- Event Handlers ----------
    //void OnGLCanvasPaint(wxPaintEvent& event);
    //void OnGLCanvasKeyDown(wxKeyEvent& event);
    //void OnGLCanvasKeyUp(wxKeyEvent& event);
    void OnGLCanvasLeftMouseButtonDown(wxMouseEvent& event);
    void OnGLCanvasLeftMouseButtonUp(wxMouseEvent& event);
    void OnGLCanvasRightMouseButtonDown(wxMouseEvent& event);
    void OnGLCanvasRightMouseButtonUp(wxMouseEvent& event);
    void OnGLCanvasMouseMove(wxMouseEvent& event);
    void OnGLCanvasMouseWheel(wxMouseEvent& event);
};

#endif // MWXGLCANVAS_MOUSE_H
