#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define USE_INTEL_GPU       // if use nVidia/AMD graphics card, comment this line out

#include "OpenGLWindow.h"
#include "MwxGLCanvas_Mouse.h"
#include "Shader.h"

#ifdef USE_INTEL_GPU
#include "Model130.h"       // For GLSL 1.30
#else
#include "Model130.h"       // For GLSL 3.30
#endif

#include "Constraints.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>

#include <wx/splitter.h>
#include <wx/propgrid/propgrid.h>
#include <wx/wx.h>
#include <wx/stdpaths.h>
#include <wx/filedlg.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "acHouseEngine.h"

class MainWindow : public OpenGLWindow
{

    wxString currentDir;

    GLuint m_VAO_ID;                // Uncomment this line later
    GLuint m_VBO_ID;                // Uncomment this line later
    GLuint m_EBO_ID;                // Uncomment this line later
    GLuint m_VAO_XYPlane_ID;
    GLuint m_VBO_XYPlane_ID;

    MwxGLCanvas*    m_GLCanvas;
    wxGLContext*    m_GLContext;
    Shader          m_shader_XYPlane;
    Shader          m_shader_UniformColor;          // For specular lighting
    Shader          m_shader_3DModel;       // Uncomment this line later
    Model*           m_ourModel;
    Model           m_ourLamp;

    wxSplitterWindow* m_acSplitter;
    //Property grid variables
    wxPGProperty* minWidth;
    wxPGProperty* maxWidth;
    wxPGProperty* minLength;
    wxPGProperty* maxLength;
    wxPGProperty* minHeight;
    wxPGProperty* maxHeight;
    wxPGProperty* rHeight;
    wxPGProperty* rOverh;
    wxPGProperty* pmaxLength;
    wxPGProperty* pmaxWidth;
    wxPGProperty* pminLength;
    wxPGProperty* pminWidth;
    
    wxPGProperty* bminHeight;
    wxPGProperty* bmaxHeight;
    wxPGProperty* bminSide;
    wxPGProperty* bmaxSide;
    wxPGProperty* bSteps;

    bool IsRunning;
    bool IsResized;
    bool m_autoRotation;
    bool m_fullScreen;
    bool m_showXYPlane;
    bool m_camLookAtOrigin;

    GLuint m_texture1_ID[4];        // Uncomment this line later
    //int    m_textureIndex;
    int    m_angle;
    int    m_modelIndex;

    glm::mat4 m_Mproj;              // Projection Matrix
    glm::mat4 m_Mview;              // Camera View Matrix
    glm::vec3 pointLightPos[2];     // Two positions of lamps
    glm::vec3 lampColor[2];         // Two colors of lamps

    // ---------- For Camera Movement ----------------------------
    glm::vec3   m_camPos, m_camFront;
    float       m_camFOVy;
    float       m_yaw, m_pitch, m_roll;
    bool        IsViewProjChanged;

    const glm::vec3 m_camUp = glm::vec3(0.0f, 0.0f, 1.0f);

    enum enumKeys
    {
        camZoomIn, camZoomOut,
        camMoveLeft, camMoveRight, camMoveUp, camMoveDown,
        camMoveForward, camMoveBackward,
        camTiltLeft, camTiltRight, camTiltUp, camTiltDown
    };

    static constexpr float PI = 3.1415927f;
    static constexpr float PI_over_180 = 3.1415927f / 180.0f;
    static constexpr int keyNumber = 10;
    bool keys[keyNumber];

    void Update_Camera_Movement(float t);     // Calculate camera movement
    void Calculate_Camera_Front(float yaw, float pitch);
    void Reset_Camera_Front(glm::vec3 VcamaFront);
    void Show_Yaw_Pitch();      // Dispaly yaw and pitch data in status bar
    // -----------------------------------------------------------

public:
    MainWindow(wxFrame* parent = 0);
    virtual ~MainWindow();

    bool initializeGL();              // Called from main.cpp
    void renderGL();
    void resizeGL(int width, int height);

protected:
    bool initLoadShader();
    bool initLoadModel();
    bool initLoadXYPlane();

private:
    // ---------- Event Handlers ----------
	void OnTimer1Trigger(wxTimerEvent& event);

	void OnPropertyGridChanged(wxPropertyGridEvent& event);
	void PropDirChange(wxString str);

	void OnMenuEditGenHouse(wxCommandEvent& event);
	void OnMenuEditSaveHouse(wxCommandEvent& event);
	void OnMenuEditGenBuild(wxCommandEvent& event);
	void OnMenuEditSaveBuild(wxCommandEvent& event);

	void OnResize(wxSizeEvent& event);
	void OnClose(wxCloseEvent& event);
	void OnMenuFileQuitSelected(wxCommandEvent& event);
	void OnMenuHelpAboutSelected(wxCommandEvent& event);
	void OnMenuViewFullScreenSelected(wxCommandEvent& event);
	void OnMenuViewShowXYPlaneSelected(wxCommandEvent& event);
	void OnMenuModelAutoRotationSelected(wxCommandEvent& event);
	void OnMenuModelRotateUpSelected(wxCommandEvent& event);
	void OnMenuModelRotateDownSelected(wxCommandEvent& event);
	void OnMenuCameraLookAtOriginSelected(wxCommandEvent& event);
	void OnMenuCameraZoomInSelected(wxCommandEvent& event);
	void OnMenuCameraZoomOutSelected(wxCommandEvent& event);
	void OnMenuCameraMoveLeftSelected(wxCommandEvent& event);
	void OnMenuCameraMoveRightSelected(wxCommandEvent& event);
	void OnMenuCameraMoveUpSelected(wxCommandEvent& event);
	void OnMenuCameraMoveDownSelected(wxCommandEvent& event);
	void OnMenuCameraMoveForwardSelected(wxCommandEvent& event);
	void OnMenuCameraMoveBackwardSelected(wxCommandEvent& event);
	void OnMenuCameraTiltLeftSelected(wxCommandEvent& event);
	void OnMenuCameraTiltRightSelected(wxCommandEvent& event);
	void OnMenuCameraTiltUpSelected(wxCommandEvent& event);
	void OnMenuCameraTiltDownSelected(wxCommandEvent& event);

	acHouseEngine *m_acEngine;
};

#endif // MAINWINDOW_H
