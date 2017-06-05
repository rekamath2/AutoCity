#ifndef MWXGLCANVAS_H
#define MWXGLCANVAS_H

#include <GL/glew.h> // Must include glew.h ahead of glcanvas.h
#include <wx/glcanvas.h>

class MwxGLCanvas : public wxGLCanvas
{
public:
    MwxGLCanvas(wxWindow* parent, int* attribList, wxSize sizeXY);
    ~MwxGLCanvas(void);

protected:
    static const long ID_GLCANVAS;

private:
    // ---------- Event Handerlers ----------
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

#endif // EWXGLCANVAS_H
