#include <iostream>
#include <wx/sizer.h>
#include <wx/msgdlg.h>

#include "MainWindow.h"

using namespace std;

MainWindow::MainWindow(wxFrame* parent)
          : OpenGLWindow(parent)
{
    //ctor
    // ----- Constructor -----
    int temp1[3] = { wxSB_SUNKEN, wxSB_SUNKEN, wxSB_SUNKEN };
    StatusBar1->SetStatusStyles(3, temp1);

    int temp2[3] = { -1, 100, 100 };
    StatusBar1->SetStatusWidths(3, temp2);

    StatusBar1->SetStatusText(_("First Field"), 0);
    StatusBar1->SetStatusText(_("Second Field"), 1);
    StatusBar1->SetStatusText(_("Third Field"), 2);

    wxBoxSizer* HBoxSizer = new wxBoxSizer(wxHORIZONTAL);
    int attribList[] = {WX_GL_RGBA,
                        WX_GL_DOUBLEBUFFER,
                        WX_GL_SAMPLE_BUFFERS,
                        GL_TRUE,
                        WX_GL_SAMPLES, 4,      // 2x2 antialiasing supersampling
                        WX_GL_DEPTH_SIZE, 16,
                        0, 0};

    //create splitter window with the gl canvas and a property grid
    m_acSplitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition,
		    wxDefaultSize, wxSP_3D);
    m_acProperty = new wxPropertyGrid(m_acSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		    wxPG_SPLITTER_AUTO_CENTER | wxPG_DEFAULT_STYLE);

    //Add properties
    m_acProperty->Append(new wxPropertyCategory("Base Dimension Constraints"));
    minWidth  = m_acProperty->Append(new wxFloatProperty("Min Width ",wxPG_LABEL, 1.0f));
    maxWidth  = m_acProperty->Append(new wxFloatProperty("Max Width ",wxPG_LABEL, 2.0f));
    minLength = m_acProperty->Append(new wxFloatProperty("Min Length",wxPG_LABEL, 2.0f));
    maxLength = m_acProperty->Append(new wxFloatProperty("Max Length",wxPG_LABEL, 4.0f));
    minHeight = m_acProperty->Append(new wxFloatProperty("Min Height",wxPG_LABEL, 1.0f));
    maxHeight = m_acProperty->Append(new wxFloatProperty("Max Height",wxPG_LABEL, 2.0f));

    m_acProperty->Append(new wxPropertyCategory("Roof Dimension Constraints"));
    rHeight = m_acProperty->Append(new wxFloatProperty("Roof Height",wxPG_LABEL, 0.5f));
    rOverh = m_acProperty->Append(new wxFloatProperty("Roof Overhang",wxPG_LABEL, 0.1f));

    m_acProperty->Append(new wxPropertyCategory("Protrusion Dimension Constraints"));
    pmaxLength = m_acProperty->Append(new wxFloatProperty("Protrusion Max Length",wxPG_LABEL,0.75));
    pmaxWidth =  m_acProperty->Append(new wxFloatProperty("Protrusion Max Width ",wxPG_LABEL,2.5));
    pminLength = m_acProperty->Append(new wxFloatProperty("Protrusion Min Length",wxPG_LABEL,0.5));
    pminWidth =  m_acProperty->Append(new wxFloatProperty("Protrusion Min Width ",wxPG_LABEL,1.0));

    m_acProperty->Append(new wxPropertyCategory("Building Dimension Constraints"));
    bminHeight = m_acProperty->Append(new wxFloatProperty("Building Min Height",wxPG_LABEL,3.0));
    bmaxHeight = m_acProperty->Append(new wxFloatProperty("Building Max Height",wxPG_LABEL,5.0));
    bminSide = m_acProperty->Append(new wxFloatProperty("Min Bottom Side Length",wxPG_LABEL,1.0));
    bmaxSide = m_acProperty->Append(new wxFloatProperty("Max Top Side Length",wxPG_LABEL,0.5));
    bSteps = m_acProperty->Append(new wxIntProperty("Number of Steps",wxPG_LABEL,5));

    m_GLCanvas = new MwxGLCanvas(m_acSplitter, attribList, wxSize(800,600));
    m_GLCanvas->SetMinSize(wxSize(320,240));

    m_acSplitter->SplitVertically(m_acProperty, m_GLCanvas, 800);


    HBoxSizer->Add(m_acSplitter, 1, wxEXPAND);
    SetSizer(HBoxSizer);
    SetAutoLayout(true);
    HBoxSizer->Fit(this);
    HBoxSizer->SetSizeHints(this);

    SetTitle(_("AutoCity"));
    SetClientSize(800, 600);
    Center();    // Move the whole window at the center of your screen

    IsRunning = false;

    m_fullScreen        = false;
    m_autoRotation      = true;
    m_camLookAtOrigin   = false;

    m_showXYPlane  = true;              // Show X-Y plane by default
    MenuViewShowXYPlane->Check(true);   // Also need to check menu item

    m_modelIndex = 0;
    m_ourModel = new Model();

    ShowFullScreen(true);       // To enable wxMenu to get key events
    ShowFullScreen(false);

    m_angle = 0;

    // ---------- For Camera Movement ----------------------------
    m_camFOVy = 0.7854f;        // 45 degrees for GLM 0.9.7 (Ubuntu 16.x / Linux Mint 18.x)

    m_camPos = glm::vec3(7.0f);

    Reset_Camera_Front(-m_camPos);      // Reet camera front vector

    for (int i = 0; i < keyNumber; i++) keys[i] = false;

    IsViewProjChanged = false;
    // -----------------------------------------------------------
    m_acSplitter->SetSashPosition(300);
}

MainWindow::~MainWindow()
{
    //free allocated memory
    delete m_GLCanvas;
    delete m_GLContext;
}

// ##### <<<<<< Beginning of Event Handler Area >>>>> #####

void MainWindow::OnTimer1Trigger(wxTimerEvent& event)
{
    if (!IsResized)     // Discard first frame for resizing
    {
        resizeGL(this->GetClientSize().GetWidth(),
                 this->GetClientSize().GetHeight());
    }
    else renderGL();    // Before being resized, do not render OpenGL
}

void MainWindow::OnPropertyGridChanged(wxPropertyGridEvent& event)
{
	wxPGProperty* prop = event.GetProperty();

	cout<<"propevent"<<endl;
	if(!prop)
		return;
	wxAny value = prop->GetValue();
	if(value.IsNull())
		return;
	if(prop->GetName() == "OutputFolder")
		PropDirChange(value.As<wxString>());
	if(prop->GetName() == "Number of Houses")
		return;
}

void MainWindow::PropDirChange(wxString str)
{
	string outpath = str.ToStdString();
	cout<<"dirchanged"<<endl;
}

void MainWindow::OnMenuEditGenHouse(wxCommandEvent& event)
{
	//Get values from property grid, store in constraints object
	wxAny value1 = minWidth->GetValue();
	wxAny value2 = maxWidth->GetValue();
	wxAny value3 = minLength->GetValue();
	wxAny value4 = maxLength->GetValue();
	wxAny value5 = minHeight->GetValue();
	wxAny value6 = maxHeight->GetValue();
	wxAny value7 = rHeight->GetValue();
	wxAny value8 = rOverh->GetValue();
	wxAny value9 = pminWidth->GetValue();
	wxAny value10 = pmaxWidth->GetValue();
	wxAny value11 = pminLength->GetValue();
	wxAny value12 = pmaxLength->GetValue();
	Constraints c(value1.As<double>(),value2.As<double>(),value3.As<double>(),
			value4.As<double>(),value5.As<double>(),value6.As<double>(),
			value7.As<double>(), value8.As<double>(),
			value9.As<double>(), value10.As<double>(),
			value11.As<double>(), value12.As<double>());
	m_acEngine = new acHouseEngine();
	m_acEngine->genHouse(c);
	delete m_ourModel;//Create new model for new obj file
	m_ourModel = new Model();
	m_ourModel->loadModel("./tmp/house.obj", m_shader_3DModel);
}

void MainWindow::OnMenuEditSaveHouse(wxCommandEvent& event)
{
	string str = "./tmp/house.obj";

	wxFileDialog saveFileDialog(this, _("Save House OBJ"), "", "untitled.obj", "OBJ files (*.obj)|*.obj", wxFD_SAVE |
			wxFD_OVERWRITE_PROMPT);

	if(saveFileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}
	string savepath = saveFileDialog.GetPath().ToStdString();

	//Save file to specified path
	ifstream source(str, ios::binary);
	ofstream dest(savepath, ios::binary);

	istreambuf_iterator<char> begin_source(source);
	istreambuf_iterator<char> end_source;
	ostreambuf_iterator<char> begin_dest(dest);
	copy(begin_source, end_source, begin_dest);

	source.close();
	dest.close();

}

void MainWindow::OnMenuEditGenBuild(wxCommandEvent& event)
{
	//store constraints in constraints object
	wxAny value1 = bminHeight->GetValue();
	wxAny value2 = bmaxHeight->GetValue();
	wxAny value3 = bminSide->GetValue();
	wxAny value4 = bmaxSide->GetValue();
	wxAny value5 = bSteps->GetValue();
	Constraints c(value1.As<double>(), value2.As<double>(), value3.As<double>(),
			value4.As<double>(), value5.As<int>());
	m_acEngine = new acHouseEngine();
	m_acEngine->genBuild(c);
	delete m_ourModel;
	m_ourModel = new Model();
	m_ourModel->loadModel("./tmp/building.obj", m_shader_3DModel);
}

void MainWindow::OnMenuEditSaveBuild(wxCommandEvent& event)
{

	//save building to specified path
	string str = "./tmp/building.obj";
	wxFileDialog saveFileDialog(this, _("Save Building OBJ"), "", "untitled.obj", "OBJ files (*.obj)|*.obj", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if(saveFileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}
	string savepath = saveFileDialog.GetPath().ToStdString();

	ifstream source(str, ios::binary);
	ofstream dest(savepath, ios::binary);

	istreambuf_iterator<char> begin_source(source);
	istreambuf_iterator<char> end_source;
	ostreambuf_iterator<char> begin_dest(dest);
	copy(begin_source, end_source, begin_dest);

	source.close();
	dest.close();

}

void MainWindow::OnMenuHelpAboutSelected(wxCommandEvent& event)
{
    wxMessageBox(_("This is AutoCity v1.1.1"),
                 _("About AutoCity"),
                 wxOK | wxCENTER | wxICON_INFORMATION);
}

void MainWindow::OnResize(wxSizeEvent& event)
{
    this->Layout();
    IsResized = false;
}

void MainWindow::OnClose(wxCloseEvent& event)
{
    cout << "***** Stop running OpenGL now !! *****" << endl;
    IsRunning = false;
    event.Skip();       // Continue closing this application
}

void MainWindow::OnMenuFileQuitSelected(wxCommandEvent& event)
{
    Close();        // This will call OnClose() event handler
}

void MainWindow::OnMenuViewFullScreenSelected(wxCommandEvent& event)
{
    m_fullScreen = !m_fullScreen;
    MenuViewFullScreen->Check(m_fullScreen);
    ShowFullScreen(m_fullScreen);
}

void MainWindow::OnMenuViewShowXYPlaneSelected(wxCommandEvent& event)
{
    m_showXYPlane = !m_showXYPlane;
    MenuViewShowXYPlane->Check(m_showXYPlane);
}

void MainWindow::OnMenuModelAutoRotationSelected(wxCommandEvent& event)
{
    m_autoRotation = !m_autoRotation;
    MenuModelAutoRotation->Check(m_autoRotation);
}

void MainWindow::OnMenuModelRotateUpSelected(wxCommandEvent& event)
{
    m_angle += 2;       // Rotate model up 2 degrees
}

void MainWindow::OnMenuModelRotateDownSelected(wxCommandEvent& event)
{
    m_angle -= 2;       // Rotate model down 2 degrees
}

void MainWindow::OnMenuCameraLookAtOriginSelected(wxCommandEvent& event)
{

    //lock camera to origin
    MenuCameraTiltLeft->Enable(m_camLookAtOrigin);
    MenuCameraTiltRight->Enable(m_camLookAtOrigin);
    MenuCameraTiltUp->Enable(m_camLookAtOrigin);
    MenuCameraTiltDown->Enable(m_camLookAtOrigin);

    m_camLookAtOrigin = !m_camLookAtOrigin;
    MenuCameraLookAtOrigin->Check(m_camLookAtOrigin);

    Reset_Camera_Front(-m_camPos);
    m_Mview = glm::lookAt(m_camPos, m_camPos + m_camFront, m_camUp);
    IsViewProjChanged = true;
}

void MainWindow::OnMenuCameraZoomInSelected(wxCommandEvent& event)
{
    keys[camZoomIn] = true;
    //cout << "OnMenuCameraZoomInSelected() is pressed" << endl;
}

void MainWindow::OnMenuCameraZoomOutSelected(wxCommandEvent& event)
{
    keys[camZoomOut] = true;
    //cout << "OnMenuCameraZoomOutSelected() is pressed" << endl;
}

void MainWindow::OnMenuCameraMoveLeftSelected(wxCommandEvent& event)
{
    keys[camMoveLeft] = true;
}

void MainWindow::OnMenuCameraMoveRightSelected(wxCommandEvent& event)
{
    keys[camMoveRight] = true;
}

void MainWindow::OnMenuCameraMoveUpSelected(wxCommandEvent& event)
{
    keys[camMoveUp] = true;
}

void MainWindow::OnMenuCameraMoveDownSelected(wxCommandEvent& event)
{
    keys[camMoveDown] = true;
}

void MainWindow::OnMenuCameraMoveForwardSelected(wxCommandEvent& event)
{
    keys[camMoveForward] = true;
}

void MainWindow::OnMenuCameraMoveBackwardSelected(wxCommandEvent& event)
{
    keys[camMoveBackward] = true;
}

void MainWindow::OnMenuCameraTiltLeftSelected(wxCommandEvent& event)
{
    if (!m_camLookAtOrigin) keys[camTiltLeft] = true;
}

void MainWindow::OnMenuCameraTiltRightSelected(wxCommandEvent& event)
{
    if (!m_camLookAtOrigin) keys[camTiltRight] = true;
}

void MainWindow::OnMenuCameraTiltUpSelected(wxCommandEvent& event)
{
    if (!m_camLookAtOrigin) keys[camTiltUp] = true;
}

void MainWindow::OnMenuCameraTiltDownSelected(wxCommandEvent& event)
{
    if (!m_camLookAtOrigin) keys[camTiltDown] = true;
}
// ##### <<<<<< End of Event Handler Area >>>>> #####

// ##### <<<<<< Beginning of User Defined Function Area >>>>> ######
bool MainWindow::initializeGL()
{
    //initialize canvas and context
    m_GLContext = new wxGLContext(m_GLCanvas);
    m_GLCanvas->SetBackgroundStyle(wxBG_STYLE_CUSTOM);
    //wxPaintDC dc(this);
    m_GLCanvas->SetCurrent(*m_GLContext);

    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE; // To expose all entry extension
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        cout << "Error initilize GLEW: " << glewGetErrorString(err) << endl;
    }

    // get version info
    const GLubyte* vendor = glGetString (GL_VENDOR);                        // get vendor string
    const GLubyte* renderer = glGetString (GL_RENDERER);                    // get renderer string
    const GLubyte* OGLversion = glGetString (GL_VERSION);                   // version as a string
    const GLubyte* GLSLversion = glGetString (GL_SHADING_LANGUAGE_VERSION); // GLSL version as a string

    cout << "Vendor: " << vendor << endl;
    cout << "Renderer: " << renderer << endl;
    cout << "OpenGL Version Supported: " << OGLversion << endl;
    cout << "GLSL Version: " << GLSLversion << endl << endl;

    glClearColor(0.2f, 0.2f, 0.23f, 1.0f);  // Background color when doing glClear
    glClearDepth(1.0f);                      // Depth buffer when doing glClear
    glDepthFunc(GL_LEQUAL);                  // The type of depth testing to do
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    bool errorFlag = false;
    if ( initLoadShader()  == false) errorFlag = true;
    if ( initLoadModel()   == false) errorFlag = true;
    if ( initLoadXYPlane() == false) errorFlag = true;
    //if ( initLoadTexture() == false) openGLFlag = false;

    if (errorFlag)
    {
        cout << "ERROR: initializeGL() was failed!  Exit this application now..." << endl;
        return false;
    }

    IsRunning = true;
    IsResized = false;
    cout << "***** Start running OpenGL now !! *****" << endl;

    return true;
}

bool MainWindow::initLoadShader()
{
    bool errorFlag = false;

#ifdef USE_INTEL_GPU
    // ---------- For GLSL 1.30 ----------
    if ((m_shader_XYPlane.loadShader((char*)("./Shader130_XYPlane.vert"),
                                (char*)("./Shader130_XYPlane.frag"))) == false)
        errorFlag = true;
    if ((m_shader_3DModel.loadShader((char*)("./Shader130_Specular.vert"),
                                (char*)("./Shader130_Specular.frag"))) == false)
        errorFlag = true;
    if ((m_shader_UniformColor.loadShader((char*)("./Shader130_UniformColor.vert"),
                                     (char*)("./Shader130_UniformColor.frag"))) == false)
        errorFlag = true;
    //cout << "$$$$> m_shader_UniformColor.Program_ID :" << m_shader_UniformColor.Program_ID << endl;
    // -----------------------------------
#else
    // ---------- For GLSL 3.30 ----------
    if ((m_shader_XYPlane.loadShader((char*)("./Shader330_XYPlane.vert"),
                                (char*)("./Shader330_XYPlane.frag"))) == false)
        errorFlag = true;
    if ((m_shader_3DModel.loadShader((char*)("./Shader330_Specular.vert"),
                                (char*)("./Shader330_Specular.frag"))) == false)
        errorFlag = true;
    if ((m_shader_UniformColor.loadShader((char*)("./Shader330_UniformColor.vert"),
                                     (char*)("./Shader330_UniformColor.frag"))) == false)
        errorFlag = true;
    // -----------------------------------
#endif

    return !errorFlag;
}

bool MainWindow::initLoadModel()
{
    bool errorFlag = false;

    wxString myString;

    myString = wxString(_("tmp/Cube.obj"));

#ifdef USE_INTEL_GPU
    // ---------- For GLSL 1.30 ----------
    if ((m_ourModel->loadModel(myString, m_shader_3DModel)) ==  false)
        errorFlag = true;
#else
    // ---------- For GLSL 3.30 ----------
    if ((m_ourModel->loadModel(myString)) ==  false)
        errorFlag = true;
#endif
    cout << "===> End of loading 3D models :-) \n" << endl;

    return !errorFlag;
}

bool MainWindow::initLoadXYPlane()
{
    const int lineNumber = 10;
    const float lineLength = 5.0f;
    const float segmentLength = lineLength / (float)(lineNumber/2);
    const int k = lineNumber + 1;
    float Vp[k*2+3][2][6];

    glLineWidth(1.0f);      // Set line width as slim as possible

    // Assgin gray color for normal grid lines
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            if ((j%2) == 0)
            {
                // Point Start ([0] -> x, [1] -> y, [2] -> z)
                if ((i - 5) != 0) Vp[i][j][0] = lineLength;
                else Vp[i][j][0] = 0.0f;
                Vp[i][j][1] = segmentLength * (float)(i - 5);
                Vp[i][j][2] = 0.0f;

                // Color ([5] -> r, [5] -> g, [5] -> b)
                Vp[i][j][5] = Vp[i][j][5] = Vp[i][j][5] = 0.2f;

                // Point Start ([0] -> x, [1] -> y, [2] -> z)
                Vp[i + k][j][0] = segmentLength * (float)(i - 5);
                if ((i - 5) != 0) Vp[i + k][j][1] = lineLength;
                else Vp[i + k][j][1] = 0.0f;
                Vp[i + k][j][2] = 0.0f;

                // Color ([5] -> r, [5] -> g, [5] -> b)
                Vp[i + k][j][5] = Vp[i + k][j][5] = Vp[i + k][j][5] = 0.2f;
            }
            else
            {
                // Point End ([0] -> x, [1] -> y, [2] -> z)
                Vp[i][j][0] = -lineLength;
                Vp[i][j][1] = segmentLength * (float)(i - 5);
                Vp[i][j][2] = 0.0f;

                // Color ([5] -> r, [5] -> g, [5] -> b)
                Vp[i][j][5] = Vp[i][j][5] = Vp[i][j][5] = 0.4f;

                // Point End ([0] -> x, [1] -> y, [2] -> z)
                Vp[i + k][j][0] = segmentLength * (float)(i - 5);
                Vp[i + k][j][1] = -lineLength;
                Vp[i + k][j][2] = 0.0f;

                // Color ([5] -> r, [5] -> g, [5] -> b)
                Vp[i + k][j][5] = Vp[i + k][j][5] = Vp[i + k][j][5] = 0.2f;
            }
        }
    }

    // Assgin red and green colors for +X and +Y coordinate axes
    for (int j = 0; j < 2; j++)
    {
        if ((j%2) == 0)
        {
            // X-axis Point Start ([0] -> x, [1] -> y, [2] -> z)
            Vp[k*2][j][0] = lineLength;
            Vp[k*2][j][1] = 0.0f;
            Vp[k*2][j][2] = 0.0f;

            // Red Color ([3] -> r, [4] -> g, [5] -> b)
            Vp[k*2][j][3] = 1.0f;
            Vp[k*2][j][4] = Vp[k*2][j][5] = 0.0f;

            // Y-axis Point Start ([0] -> x, [1] -> y, [2] -> z)
            Vp[k*2+1][j][0] = 0.0f;
            Vp[k*2+1][j][1] = lineLength;
            Vp[k*2+1][j][2] = 0.0f;

            // Green Color ([3] -> r, [4] -> g, [5] -> b)
            Vp[k*2+1][j][3] = Vp[k*2+1][j][5] = 0.0f;
            Vp[k*2+1][j][4] = 1.0f;
        }
        else
        {
            // X-axis Point End ([0] -> x, [1] -> y, [2] -> z)
            Vp[k*2][j][0] = 0.0f;
            Vp[k*2][j][1] = 0.0f;
            Vp[k*2][j][2] = 0.0f;

            // Red Color ([3] -> r, [4] -> g, [5] -> b)
            Vp[k*2][j][3] = 1.0f;
            Vp[k*2][j][4] = Vp[k*2][j][5] = 0.0f;

            // Y-axis Point End ([0] -> x, [1] -> y, [2] -> z)
            Vp[k*2+1][j][0] = 0.0f;
            Vp[k*2+1][j][1] = 0.0f;
            Vp[k*2+1][j][2] = 0.0f;

            // Green Color ([3] -> r, [4] -> g, [5] -> b)
            Vp[k*2+1][j][3] = Vp[k*2+1][j][5] = 0.0f;
            Vp[k*2+1][j][4] = 1.0f;
        }
    }

    // Assign blue color for +Z coordinate axis
    // Z-axis Point Start ([0] -> x, [1] -> y, [2] -> z)
    Vp[k*2+2][0][0] = 0.0f;
    Vp[k*2+2][0][1] = 0.0f;
    Vp[k*2+2][0][2] = lineLength;

    // Green Color ([3] -> r, [4] -> g, [5] -> b)
    Vp[k*2+2][0][3] = Vp[k*2+2][0][4] = 0.0f;
    Vp[k*2+2][0][5] = 1.0f;

    // Z-axis Point End ([0] -> x, [1] -> y, [2] -> z)
    Vp[k*2+2][1][0] = 0.0f;
    Vp[k*2+2][1][1] = 0.0f;
    Vp[k*2+2][1][2] = 0.0f;

    // Green Color  ([3] -> r, [4] -> g, [5] -> b)
    Vp[k*2+2][1][3] = Vp[k*2+2][1][4] = 0.0f;
    Vp[k*2+2][1][5] = 1.0f;


    //GLuint VBO_XYPlane, VAO_XYPlane;
    glGenVertexArrays(1, &m_VAO_XYPlane_ID);  // Assign a number to m_VAO_ID

    // Bind our Vertex Array Object first,
    // then bind and set our buffers and pointers.
    glBindVertexArray(m_VAO_XYPlane_ID); // Bind below commands

    glGenBuffers(1, &m_VBO_XYPlane_ID); // Assign a number to m_VBO_XYPlane_ID

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO_XYPlane_ID);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(Vp),
                 Vp,
                 GL_STATIC_DRAW);

    // ---=== Setup shader ports and data reading parameters ===---
    GLint Loc;
    // Position attribute
    Loc = glGetAttribLocation( m_shader_XYPlane.Program_ID, "position" );
    glVertexAttribPointer(Loc, 3, GL_FLOAT, GL_FALSE,
                               6 * sizeof(GLfloat),      // Span of a data set
                      (GLvoid*)0);                       // Offset to first data
    glEnableVertexAttribArray(Loc);   // Enable first shader variable (position)

    // Color attribute
    Loc = glGetAttribLocation( m_shader_XYPlane.Program_ID, "color" );
    glVertexAttribPointer(Loc, 3, GL_FLOAT, GL_FALSE,
                               6 * sizeof(GLfloat),      // Span of a data set
                     (GLvoid*)(3 * sizeof(GLfloat)));    // Offset to first data
    glEnableVertexAttribArray(Loc);   // Enable second shader variable (color)
    // Unbind VAO (it's always a good thing to unbind any buffer/array
    // to prevent strange bugs)
    glBindVertexArray(0);

    return true;
}
void MainWindow::renderGL()
{
    //only render if ready
    if (!IsRunning || IsIconized()) return;

    //wxPaintDC(this);
    m_GLCanvas->SetCurrent(*m_GLContext);
    //glClearColor(1.0, 0.0, 0.0, 1.0);   // Change any color you like here

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //initialize variables to read shader data

    static glm::mat4 Mvp;
    static bool     IsLocReady(false);
    static bool     IsViewProjChanged_XYPlane(false);
    static bool     IsViewProjChanged_Lamp(false);
    static GLint    Loc_Mmvp_XYPlane;
    static GLint    Loc_Mmvp;
    static GLint    Loc_Mmvp_Lamp;
    static GLint    Loc_lampColor;
    static GLint    Loc_Mmodel;
    static GLint    Loc_Mtranspose_inverse_model;
    static GLint    Loc_viewPos;
    static int      count(0);
    static int      counter_render;

    static wxStopWatch stopWatch;
    static wxLongLong timeNew, timeOld;
    static float angleRotation(0.0f);       // Angle of auto-rotation

    wxLongLong  t_delta;      // Time difference in micro-second
    float       dt;           // Time difference in second

    GLint Loc;
/**/
    //if ((counter_render % 330) == 0)

    if (!IsLocReady)    // Run only once to get shader location
    {
        // –--- Calculate Camera Matrix -----
        //glm::vec3 cameraPos = glm::vec3(2.0f, 2.0f, 2.0f);
        //glm::vec3 cameraFront = glm::vec3(-1.0f, -1.0f, -1.0f);
        //glm::vec3 cameraPos = glm::vec3(3.0f, 1.0f, 3.0f);
        //glm::vec3 cameraFront = -cameraPos;
        //glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, 1.0f);
        //m_Mview = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        m_Mview = glm::lookAt(m_camPos, m_camPos + m_camFront, m_camUp);
        IsViewProjChanged = true;
        // ----------------------------------

        // ========== Setup variable locations for shaders of XYPlane ==========
        Loc_Mmvp_XYPlane = glGetUniformLocation(m_shader_XYPlane.Program_ID, "Mmvp");

        // ===================================================================

        // ========== Setup variable locations for shaders of UniformColor ==========
        //pointLightPos[0] = glm::vec3(2.3f, -1.6f, -3.0f);
        //pointLightPos[0] = glm::vec3(0.0f);
        //pointLightPos[0] = glm::vec3(2.0f);
        pointLightPos[0] = glm::vec3(2.0f, 1.5f, 3.5f);

        //pointLightPos[1] = glm::vec3(-1.7f, 0.9f, 1.0f);
        //pointLightPos[1] = glm::vec3(0.0f);
        pointLightPos[1] = glm::vec3(-3.0f, -1.5f, -1.5f);

        lampColor[0] = glm::vec3(1.0f, 1.0f, 1.0f);
        lampColor[1] = glm::vec3(1.0f, 1.0f, 1.0f);
/**/
        Loc_Mmvp_Lamp = glGetUniformLocation(m_shader_UniformColor.Program_ID, "Mmvp");
//cout << "Loc_Mmvp = " << Loc_Mmvp_Lamp << endl;
        Loc_lampColor = glGetUniformLocation(m_shader_UniformColor.Program_ID, "ourColor");
//cout << "Loc_lampColor = " << Loc_lampColor << endl;
        //m_shader_UniformColor.useProgram();
        //glUniform3fv(Loc_lampColor, 1, glm::value_ptr(lampColor[0]));  // If both lamp colors are the same

        // ===================================================================

        // ========== Setup variable locations for shaders of 3D model ==========
        // -----====== Assign Texture Layer(s) to Uniform Location(s) ======-----
        m_shader_3DModel.useProgram();    // Call glUseProgram() before calling glUniform1i()
        Loc = glGetUniformLocation(m_shader_3DModel.Program_ID, "ourTexture1");
        glUniform1i(Loc, 0);               // Texture layer 0 is for ourTexture1

        glm::vec3 tempColor;
        // ----- Point Light 1 -----
        Loc = glGetUniformLocation(m_shader_3DModel.Program_ID, "pointLights[0].position");
        glUniform3fv(Loc, 1, glm::value_ptr(pointLightPos[0]));
//cout << Loc << endl;
        Loc = glGetUniformLocation(m_shader_3DModel.Program_ID, "pointLights[0].ambient");
        glUniform3f(Loc, 0.04f, 0.04f, 0.04f);
//cout << Loc << endl;
        Loc = glGetUniformLocation(m_shader_3DModel.Program_ID, "pointLights[0].diffuse");
        tempColor = lampColor[0] / 2.0f;
        glUniform3fv(Loc, 1, glm::value_ptr(tempColor));
//cout << Loc << endl;
        Loc = glGetUniformLocation(m_shader_3DModel.Program_ID, "pointLights[0].specular");
        glUniform3fv(Loc, 1, glm::value_ptr(lampColor[0]));
//cout << Loc << endl;
        Loc = glGetUniformLocation(m_shader_3DModel.Program_ID, "pointLights[0].constant");
        glUniform1f(Loc, 1.0f);
//cout << Loc << endl;
        Loc = glGetUniformLocation(m_shader_3DModel.Program_ID, "pointLights[0].linear");
        glUniform1f(Loc, 0.009);
//cout << Loc << endl;
        Loc = glGetUniformLocation(m_shader_3DModel.Program_ID, "pointLights[0].quadratic");
        glUniform1f(Loc, 0.0032);
//cout << Loc << endl;
        // ----- Point Light 2 -----
        Loc = glGetUniformLocation(m_shader_3DModel.Program_ID, "pointLights[1].position");
        glUniform3fv(Loc, 1, glm::value_ptr(pointLightPos[1]));
//cout << Loc << endl;
        Loc = glGetUniformLocation(m_shader_3DModel.Program_ID, "pointLights[1].ambient");
        glUniform3f(Loc, 0.04f, 0.04f, 0.04f);
//cout << Loc << endl;
        Loc = glGetUniformLocation(m_shader_3DModel.Program_ID, "pointLights[1].diffuse");
        tempColor = lampColor[1] / 2.0f;
        glUniform3fv(Loc, 1, glm::value_ptr(tempColor));
//cout << Loc << endl;
        Loc = glGetUniformLocation(m_shader_3DModel.Program_ID, "pointLights[1].specular");
        glUniform3fv(Loc, 1, glm::value_ptr(lampColor[1]));
//cout << Loc << endl;
        Loc = glGetUniformLocation(m_shader_3DModel.Program_ID, "pointLights[1].constant");
        glUniform1f(Loc, 1.0f);
//cout << Loc << endl;
        Loc = glGetUniformLocation(m_shader_3DModel.Program_ID, "pointLights[1].linear");
        glUniform1f(Loc, 0.009);
//cout << Loc << endl;
        Loc = glGetUniformLocation(m_shader_3DModel.Program_ID, "pointLights[1].quadratic");
        glUniform1f(Loc, 0.0032);
//cout << Loc << endl;
        // ----------------------------------------------------------------------

        Loc_Mmvp = glGetUniformLocation(m_shader_3DModel.Program_ID, "Mmvp");
        //cout << "Loc_Mmvp : " << Loc_Mmvp << endl;
        //Loc_Mmodel = glGetUniformLocation(m_shader_3DModel.Program_ID, "Mmodel");
        Loc_viewPos = glGetUniformLocation(m_shader_3DModel.Program_ID, "viewPos");
        //cout << "Loc_viewPos : " << Loc_viewPos << endl;
        Loc_Mmodel = glGetUniformLocation(m_shader_3DModel.Program_ID, "Mmodel");       // Why this statement was missing 02-21-2017
        Loc_Mtranspose_inverse_model = glGetUniformLocation(m_shader_3DModel.Program_ID, "Mtranspose_inverse_model"); // Added 02-21-2017
        // =======================================================================

        timeOld = stopWatch.TimeInMicro();

        IsLocReady = true;
        counter_render = 0;
    }

    // Measure actual time interval between frames
    // If required, display the time when m_autoRotation is true
    timeNew = stopWatch.TimeInMicro();
    t_delta = timeNew - timeOld;
    timeOld = timeNew;
    dt = (float)(t_delta.ToDouble()) * 1e-6;
/*
    if (counter_render < 10)
    {
        cout << "frame number = " << counter_render << ", dt = " << dt << ", t_delta = " << t_delta.ToDouble() << endl;
    }
    counter_render++;
*/
    Update_Camera_Movement(dt);      // Process keyboard and mouse events

    if (IsViewProjChanged)
    {
        Mvp = m_Mproj * m_Mview;
        IsViewProjChanged = false;
        IsViewProjChanged_XYPlane = true;
        IsViewProjChanged_Lamp = true;
    }

    if (m_showXYPlane)
    {
        m_shader_XYPlane.useProgram();      // ---> Select shader for X-Y plane <---
        if (IsViewProjChanged_XYPlane)
        {
            glm::mat4 Mmvp = Mvp;
            glUniformMatrix4fv(Loc_Mmvp_XYPlane, 1, GL_FALSE, glm::value_ptr(Mmvp));
            IsViewProjChanged_XYPlane = false;
        }

        glBindVertexArray(m_VAO_XYPlane_ID);
        glDrawArrays(GL_LINES, 0, 50);
        glBindVertexArray(0);
    }
/**/
    // ---------- Draw Point Lights / Lamps ----------
    m_shader_UniformColor.useProgram();

    for (int i=0; i<2; i++)
    {
        glm::mat4 Mmvp(1.0f);
        Mmvp = Mvp * glm::translate(glm::mat4(1.0f), pointLightPos[i]);
        Mmvp = glm::scale(Mmvp, glm::vec3(0.1f, 0.1f, 0.1f));
        //Mmvp = glm::scale(Mmvp, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(Loc_Mmvp_Lamp, 1, GL_FALSE, glm::value_ptr(Mmvp));
        //glUniformMatrix4fv(Loc_Mmvp_Lamp, 1, GL_FALSE, &Mmvp[0][0]);
        glUniform3fv(Loc_lampColor, 1, glm::value_ptr(lampColor[i]));
        //glUniform3fv(Loc_lampColor, 1, &lampColor[i].x);
        //glUniform3f(Loc_lampColor, 1.0f, 1.0f, 1.0f);
        //m_ourLamp.Draw(m_shader_UniformColor);
        m_ourLamp.Draw(m_shader_UniformColor);
    }

    // -----------------------------------------------

    if (m_autoRotation)     // Rotae 3D Model in world space
    {
        //angleRotation += 62.5f * dt;          // 62.5 degrees/sec for GLM 0.9.5 (Ubuntu 14.x / Linux Mint 17.x)
        //if (angleRotation > 360.0f) angleRotation -= 360.0f;

        angleRotation += 1.0908f * dt;          // 62.5 degrees/sec for GLM 0.9.7 (Ubuntu 16.x / Linux Mint 18.x)
        if (angleRotation > 6.28318f) angleRotation -= 6.28318f;

        //if ((count % 10) == 0) cout << angleRotation << endl;
        //if (count >= 360) count -= 360;
        //if ((count % 10) == 0) cout << count << endl;
        //if ((count % 10) == 0) cout << t_delta.ToString().ToUTF8() << endl;
        //if (count < 30) cout << angleRotation << endl;
        //count++;
    }

    m_shader_3DModel.useProgram();      // ---> Select shader for 3D model <---

    // Draw the loaded model
    glm::mat4 Mmodel;
    Mmodel = glm::rotate(Mmodel, angleRotation, glm::vec3(0.0f, 0.0f, 1.0f));
    Mmodel = glm::rotate(Mmodel, (float)m_angle * 0.0174533f, glm::vec3(1.0f, 0.0f, 0.0f));     // <-- 45 degree for GLM v0.9.7.2
    glUniformMatrix4fv(Loc_Mmodel, 1, GL_FALSE, glm::value_ptr(Mmodel));
    glUniformMatrix4fv(Loc_Mmvp, 1, GL_FALSE, glm::value_ptr(Mvp * Mmodel));

    // Calculate the transpose-inverse matrix of Mmodel
    Mmodel = glm::transpose(glm::inverse(Mmodel));
    glUniformMatrix4fv(Loc_Mtranspose_inverse_model, 1, GL_FALSE, glm::value_ptr(Mmodel));

    // Set the lighting uniforms
    glUniform3fv(Loc_viewPos, 1, glm::value_ptr(m_camPos));
    m_ourModel->Draw(m_shader_3DModel);

    // Swap the buffers
    m_GLCanvas->SwapBuffers();
}

void MainWindow::resizeGL(int w, int h)
{
    m_Mproj = glm::perspective(m_camFOVy, (float)w/(float)h, 0.2f, 50.0f);
    IsViewProjChanged = true;

    glViewport(0, 0, w, h);
    m_GLCanvas->SetSize(w, h);

    IsResized = true;
}

void MainWindow::Update_Camera_Movement(float t)
{
    // ---------- Mouse Tilt and Zoom -------------------------------
    if (m_GLCanvas->IsLMBPressed)
    {
        if (!m_camLookAtOrigin) // if not looking at origin, camera can tilt freely.
        {
            m_yaw     += 0.002f * m_camFOVy * 57.296f * (float)m_GLCanvas->dXmouse;
            if (m_yaw > 360.0f) m_yaw -= 360.0f;
            if (m_yaw < -360.0f) m_yaw += 360.0f;

            m_pitch   += 0.002f * m_camFOVy * 57.296f * (float)m_GLCanvas->dYmouse;
            if (m_pitch < -89.0f) m_pitch = -89.0f;
            if (m_pitch >  89.0f) m_pitch =  89.0f;

            Calculate_Camera_Front(m_yaw, m_pitch);
            m_Mview = glm::lookAt(m_camPos, m_camPos + m_camFront, m_camUp);
            IsViewProjChanged = true;
        }
        m_GLCanvas->dXmouse = m_GLCanvas->dYmouse = 0;
        Show_Yaw_Pitch();
    }

    if (m_GLCanvas->IsMWheelRotated)
    {
        /*
        // ---------------- For GLM v0.9.5 (Ubuntu 14.x / Linux Mint 17.x) ---------------------
        m_camFOVy +=  1.0f * (float)m_GLCanvas->dZmouse;
        if (m_camFOVy > 120.0f) m_camFOVy = 120.0f;
        if (m_camFOVy <   5.0f) m_camFOVy =   5.0f;
        */

        // ---------------- For GLM v0.9.7 (Ubuntu 16.x / Linux Mint 18.x) ---------------------
        m_camFOVy +=  0.01745f * (float)m_GLCanvas->dZmouse;
        if (m_camFOVy > 2.0944f) m_camFOVy = 2.0944f;
        if (m_camFOVy < 8.7266e-2f) m_camFOVy = 8.7266e-2f;

        int w = this->GetClientSize().GetWidth();
        int h = this->GetClientSize().GetHeight();
        m_Mproj = glm::perspective(m_camFOVy, (float)w/(float)h, 0.2f, 50.0f);
        IsViewProjChanged = true;

        m_GLCanvas->IsMWheelRotated = false;    // Reset dZmouse
    }
    // --------------------------------------------------------------

    // ---------- Camera Zoom (Changing m_camFOVy and m_Mproj) ------------
    if (keys[camZoomIn])
    {
        //if (m_camFOVy > 5.0f) m_camFOVy -= 0.5f;                // min 5 degrees for GLM v0.9.5 (Ubuntu 14.x / Linux Mint 17.x)
        if (m_camFOVy > 8.72664e-3f) m_camFOVy -= 8.72664e-3f;    // min 5 degrees for GLM v0.9.7 (Ubuntu 16.x / Linux Mint 18.x)
        int w = this->GetClientSize().GetWidth();
        int h = this->GetClientSize().GetHeight();
        m_Mproj = glm::perspective(m_camFOVy, (float)w/(float)h, 0.2f, 50.0f);
        IsViewProjChanged = true;

        if (!wxGetKeyState(wxKeyCode('='))) keys[camZoomIn] = false;
        //cout << "keys[camZoomIn] = " << keys[camZoomIn] << endl;
    }

    if (keys[camZoomOut])
    {
        //if (m_camFOVy < 120.0f) m_camFOVy += 0.5f;          // max 120 degrees for GLM v0.9.5 (Ubuntu 14.x / Linux Mint 17.x)
        if (m_camFOVy < 2.0944f) m_camFOVy += 8.72664e-3f;   // max 120 degrees for GLM v0.9.7 (Ubuntu 16.x / Linux Mint 18.x)
        int w = this->GetClientSize().GetWidth();
        int h = this->GetClientSize().GetHeight();
        m_Mproj = glm::perspective(m_camFOVy, (float)w/(float)h, 0.2f, 50.0f);
        IsViewProjChanged = true;

        if (!wxGetKeyState(wxKeyCode('-'))) keys[camZoomOut] = false;
        //cout << "keys[camZoomOut] = " << keys[camZoomOut] << endl;
    }
    // -------------------------------------------

    // ---------- Camera Tilt (Changing m_camFront) ----------
    if (keys[camTiltLeft])
    {
        m_yaw += 0.5f;
        if (m_yaw > 360.0f) m_yaw -= 360.0f;
        Calculate_Camera_Front(m_yaw, m_pitch);
        m_Mview = glm::lookAt(m_camPos, m_camPos + m_camFront, m_camUp);
        IsViewProjChanged = true;

        if (!(wxGetKeyState(WXK_CONTROL) && wxGetKeyState(WXK_LEFT)))
            keys[camTiltLeft] = false;
        //cout << "keys[camTiltLeft] = " << keys[camTiltLeft] << endl;
        Show_Yaw_Pitch();
    }

    if (keys[camTiltRight])
    {
        m_yaw -= 0.5f;
        if (m_yaw < -360.0f) m_yaw += 360.0f;
        Calculate_Camera_Front(m_yaw, m_pitch);
        m_Mview = glm::lookAt(m_camPos, m_camPos + m_camFront, m_camUp);
        IsViewProjChanged = true;

        if (!(wxGetKeyState(WXK_CONTROL) && wxGetKeyState(WXK_RIGHT)))
            keys[camTiltRight] = false;
        //cout << "keys[camTiltRight] = " << keys[camTiltRight] << endl;
        Show_Yaw_Pitch();
    }

    if (keys[camTiltUp])
    {
        m_pitch += 0.5f;
        if (m_pitch < -89.0f) m_pitch = -89.0f;
        Calculate_Camera_Front(m_yaw, m_pitch);
        m_Mview = glm::lookAt(m_camPos, m_camPos + m_camFront, m_camUp);
        IsViewProjChanged = true;

        if (!(wxGetKeyState(WXK_CONTROL) && wxGetKeyState(WXK_UP)))
            keys[camTiltUp] = false;
        //cout << "keys[camTiltUp] = " << keys[camTiltUp] << endl;
        Show_Yaw_Pitch();
    }

    if (keys[camTiltDown])
    {
        m_pitch -= 0.5f;
        if (m_pitch >  89.0f) m_pitch =  89.0f;
        Calculate_Camera_Front(m_yaw, m_pitch);
        m_Mview = glm::lookAt(m_camPos, m_camPos + m_camFront, m_camUp);
        IsViewProjChanged = true;

        if (!(wxGetKeyState(WXK_CONTROL) && wxGetKeyState(WXK_DOWN)))
            keys[camTiltDown] = false;
        //cout << "keys[camTiltDown] = " << keys[camTiltDown] << endl;
        Show_Yaw_Pitch();
    }
    // -------------------------------------------------------

    // ---------- Camera Move (Changing m_camPos and m_Mview) ----------
    if (keys[camMoveLeft])
    {
        //cout << "CameraPos = (" << m_camPos.x << ", " << m_camPos.y << ", " << m_camPos.z << ")" << endl;

        float Dmove = 3.0f * t;     // Moving Distance
        m_camPos -= glm::normalize(glm::cross(m_camFront, m_camUp)) * Dmove;
        if (m_camLookAtOrigin) Reset_Camera_Front(-m_camPos);
        m_Mview = glm::lookAt(m_camPos, m_camPos + m_camFront, m_camUp);
        IsViewProjChanged = true;

        if (!wxGetKeyState(wxKeyCode('A'))) keys[camMoveLeft] = false;
        //cout << "keys[camMoveLeft] = " << keys[camMoveLeft] << endl;
    }

    if (keys[camMoveRight])
    {
        float Dmove = 3.0f * t;     // Moving Distance
        m_camPos += glm::normalize(glm::cross(m_camFront, m_camUp)) * Dmove;
        if (m_camLookAtOrigin) Reset_Camera_Front(-m_camPos);
        m_Mview = glm::lookAt(m_camPos, m_camPos + m_camFront, m_camUp);
        IsViewProjChanged = true;

        if (!wxGetKeyState(wxKeyCode('D'))) keys[camMoveRight] = false;
        //cout << "keys[camMoveRight] = " << keys[camMoveRight] << endl;
    }

    if (keys[camMoveUp])
    {
        float Dmove = 3.0f * t;     // Moving Distance
        glm::vec3 camLeft = glm::cross(m_camFront, m_camUp);
        if (m_pitch > -89.0f)
        {
            m_camPos += glm::normalize(glm::cross(camLeft, m_camFront)) * Dmove;
            if (m_camLookAtOrigin) Reset_Camera_Front(-m_camPos);
            //cout << "camMoveUp:   m_pitch = " << m_pitch << endl;
        }

        m_Mview = glm::lookAt(m_camPos, m_camPos + m_camFront, m_camUp);
        IsViewProjChanged = true;
        if (!wxGetKeyState(wxKeyCode('W'))) keys[camMoveUp] = false;
        //cout << "keys[camMoveUp] = " << keys[camMoveUp] << endl;
    }

    if (keys[camMoveDown])
    {
        float Dmove = 3.0f * t;     // Moving Distance
        glm::vec3 camLeft = glm::cross(m_camFront, m_camUp);
        if (m_pitch <  89.0f)
        {
            m_camPos -= glm::normalize(glm::cross(camLeft, m_camFront)) * Dmove;
            if (m_camLookAtOrigin) Reset_Camera_Front(-m_camPos);
            //cout << "camMoveDown: m_pitch = " << m_pitch << endl;
        }

        m_Mview = glm::lookAt(m_camPos, m_camPos + m_camFront, m_camUp);
        IsViewProjChanged = true;
        if (!wxGetKeyState(wxKeyCode('S'))) keys[camMoveDown] = false;
        //cout << "keys[camMoveDown] = " << keys[camMoveDown] << endl;
    }

    if (keys[camMoveForward])
    {
        float Dmove = 3.0f * t;     // Moving Distance
        m_camPos += m_camFront * Dmove;
        if (m_camLookAtOrigin) Reset_Camera_Front(-m_camPos);
        m_Mview = glm::lookAt(m_camPos, m_camPos + m_camFront, m_camUp);
        IsViewProjChanged = true;
        if (!wxGetKeyState(wxKeyCode('Z'))) keys[camMoveForward] = false;
        //cout << "keys[camMoveForward] = " << keys[camMoveForward] << endl;
    }

    if (keys[camMoveBackward])
    {
        float Dmove = 3.0f * t;     // Moving Distance
        m_camPos -= m_camFront * Dmove;
        if (m_camLookAtOrigin) Reset_Camera_Front(-m_camPos);
        m_Mview = glm::lookAt(m_camPos, m_camPos + m_camFront, m_camUp);
        IsViewProjChanged = true;
        if (!wxGetKeyState(wxKeyCode('X'))) keys[camMoveBackward] = false;
        //cout << "keys[camMoveBackward] = " << keys[camMoveBackward] << endl;
    }
    // ----------------------------------------------------
}

void MainWindow::Calculate_Camera_Front(float yaw, float pitch)
{
	//use glm math functions to calculate camera dimensions
	glm::vec3 front;
	front.x = -cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = -sin(glm::radians(yaw)) * cos(glm::radians(pitch)) ;
	front.z =  sin(glm::radians(pitch));
	m_camFront = front;
	//cout << "Angles = (" << m_yaw<< ", " << m_pitch << ", " << m_roll << ")" << endl;
	//cout << "camerFront = (" << front.x << ", " << front.y << ", " << front.z << ")" << endl;
}

void MainWindow::Reset_Camera_Front(glm::vec3 VcamFront)
{
    m_camFront = glm::normalize(VcamFront);
	//cout << "camerFront = (" << VcamFront.x << ", " << VcamFront.y << ", " << VcamFront.z << ")" << endl;

    m_yaw = glm::atan(-VcamFront.y, -VcamFront.x) / PI_over_180;
    m_pitch = glm::atan(VcamFront.z,
              glm::sqrt(VcamFront.x * VcamFront.x +
                        VcamFront.y * VcamFront.y)) / PI_over_180;
    if (m_pitch >  89.0) m_pitch =  89.0f;
    if (m_pitch < -89.0) m_pitch = -89.0f;
    //cout << "yaw = " << m_yaw << ", pitch = " << m_pitch << endl;
    Show_Yaw_Pitch();
}

void MainWindow::Show_Yaw_Pitch()
{
    // Display four digits after "." for float numbers in status bar
    StatusBar1->SetStatusText(wxString::Format(wxT("yaw = %.4f")  , m_yaw)  , 1);
    StatusBar1->SetStatusText(wxString::Format(wxT("pitch = %.4f"), m_pitch), 2);
}
// ##### <<<<<< End of User Defined Function Area >>>>> #####
