#include "OpenGLWindow.h"

//(*InternalHeaders(OpenGLWindow)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(OpenGLWindow)
const long OpenGLWindow::ID_TIMER1 = wxNewId();
const long OpenGLWindow::ID_VIEWFULLSCREEN = wxNewId();
const long OpenGLWindow::ID_VIEWSHOWXYPLANE = wxNewId();
const long OpenGLWindow::ID_VIEWMODEL1 = wxNewId();
const long OpenGLWindow::ID_VIEWMODEL2 = wxNewId();
const long OpenGLWindow::ID_VIEWMODEL3 = wxNewId();
const long OpenGLWindow::ID_MODELAUTOROTATION = wxNewId();
const long OpenGLWindow::ID_MODELROTATEUP = wxNewId();
const long OpenGLWindow::ID_MODELROTATEDOWN = wxNewId();
const long OpenGLWindow::ID_CAMERALOOKATORIGIN = wxNewId();
const long OpenGLWindow::ID_CAMERAZOOMIN = wxNewId();
const long OpenGLWindow::ID_CAMERAZOOMOUT = wxNewId();
const long OpenGLWindow::ID_CAMERAMOVELEFT = wxNewId();
const long OpenGLWindow::ID_CAMERAMOVERIGHT = wxNewId();
const long OpenGLWindow::ID_CAMERAMOVEUP = wxNewId();
const long OpenGLWindow::ID_CAMERAMOVEDOWN = wxNewId();
const long OpenGLWindow::ID_CAMERAMOVEFORWARD = wxNewId();
const long OpenGLWindow::ID_CAMERAMOVEBACKWARD = wxNewId();
const long OpenGLWindow::ID_CAMERATILTLEFT = wxNewId();
const long OpenGLWindow::ID_CAMERATILTRIGHT = wxNewId();
const long OpenGLWindow::ID_CAMERATILTUP = wxNewId();
const long OpenGLWindow::ID_CAMERATILTDOWN = wxNewId();
const long OpenGLWindow::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(OpenGLWindow,wxFrame)
	//(*EventTable(OpenGLWindow)
	//*)
END_EVENT_TABLE()

OpenGLWindow::OpenGLWindow(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(OpenGLWindow)
	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	Timer1.SetOwner(this, ID_TIMER1);
	Timer1.Start(16, false);
	MenuBar1 = new wxMenuBar();
	Menu1 = new wxMenu();
	MenuFileQuit = new wxMenuItem(Menu1, wxID_EXIT, _("Quit\tAlt+F4"), _("Quit this application"), wxITEM_NORMAL);
	Menu1->Append(MenuFileQuit);
	MenuBar1->Append(Menu1, _("&File"));
	Menu5 = new wxMenu();
	MenuViewFullScreen = new wxMenuItem(Menu5, ID_VIEWFULLSCREEN, _("Full / Normal Screen\tF1"), wxEmptyString, wxITEM_CHECK);
	Menu5->Append(MenuViewFullScreen);
	MenuViewShowXYPlane = new wxMenuItem(Menu5, ID_VIEWSHOWXYPLANE, _("Show / Hide X-Y Plane\tP"), wxEmptyString, wxITEM_CHECK);
	Menu5->Append(MenuViewShowXYPlane);
	Menu5->AppendSeparator();
	MenuViewModel1 = new wxMenuItem(Menu5, ID_VIEWMODEL1, _("Model 1\t1"), _("Show Model 1"), wxITEM_RADIO);
	Menu5->Append(MenuViewModel1);
	MenuViewModel2 = new wxMenuItem(Menu5, ID_VIEWMODEL2, _("Model 2\t2"), _("Show Model 2"), wxITEM_RADIO);
	Menu5->Append(MenuViewModel2);
	MenuViewModel3 = new wxMenuItem(Menu5, ID_VIEWMODEL3, _("Model 3\t3"), _("Show Model 3"), wxITEM_RADIO);
	Menu5->Append(MenuViewModel3);
	MenuBar1->Append(Menu5, _("&View"));
	Menu3 = new wxMenu();
	MenuModelAutoRotation = new wxMenuItem(Menu3, ID_MODELAUTOROTATION, _("Toggle Auto Rotation\tR"), _("Toggle automatic rotation or not"), wxITEM_CHECK);
	Menu3->Append(MenuModelAutoRotation);
	Menu3->AppendSeparator();
	MenuModelRotateUp = new wxMenuItem(Menu3, ID_MODELROTATEUP, _("Rotate Up\tI"), _("Rotate up five degree"), wxITEM_NORMAL);
	Menu3->Append(MenuModelRotateUp);
	MenuModelRotateDown = new wxMenuItem(Menu3, ID_MODELROTATEDOWN, _("Rotate Down\tK"), _("Rotate down five degree"), wxITEM_NORMAL);
	Menu3->Append(MenuModelRotateDown);
	MenuBar1->Append(Menu3, _("&Model"));
	Menu6 = new wxMenu();
	MenuCameraLookAtOrigin = new wxMenuItem(Menu6, ID_CAMERALOOKATORIGIN, _("Look at Origin\tCtrl+Home"), wxEmptyString, wxITEM_CHECK);
	Menu6->Append(MenuCameraLookAtOrigin);
	MenuCameraZoomIn = new wxMenuItem(Menu6, ID_CAMERAZOOMIN, _("Zoom In\t="), wxEmptyString, wxITEM_NORMAL);
	Menu6->Append(MenuCameraZoomIn);
	MenuCameraZoomOut = new wxMenuItem(Menu6, ID_CAMERAZOOMOUT, _("Zoom Out\t-"), wxEmptyString, wxITEM_NORMAL);
	Menu6->Append(MenuCameraZoomOut);
	Menu6->AppendSeparator();
	MenuCameraMoveLeft = new wxMenuItem(Menu6, ID_CAMERAMOVELEFT, _("Move Left\tA"), wxEmptyString, wxITEM_NORMAL);
	Menu6->Append(MenuCameraMoveLeft);
	MenuCameraMoveRight = new wxMenuItem(Menu6, ID_CAMERAMOVERIGHT, _("Move Right\tD"), wxEmptyString, wxITEM_NORMAL);
	Menu6->Append(MenuCameraMoveRight);
	MenuCameraMoveUp = new wxMenuItem(Menu6, ID_CAMERAMOVEUP, _("Move Up\tW"), wxEmptyString, wxITEM_NORMAL);
	Menu6->Append(MenuCameraMoveUp);
	MenuCameraMoveDown = new wxMenuItem(Menu6, ID_CAMERAMOVEDOWN, _("Move Down\tS"), wxEmptyString, wxITEM_NORMAL);
	Menu6->Append(MenuCameraMoveDown);
	MenuCameraMoveForward = new wxMenuItem(Menu6, ID_CAMERAMOVEFORWARD, _("Move Forward\tZ"), wxEmptyString, wxITEM_NORMAL);
	Menu6->Append(MenuCameraMoveForward);
	MenuCameraMoveBackward = new wxMenuItem(Menu6, ID_CAMERAMOVEBACKWARD, _("Move Backward\tX"), wxEmptyString, wxITEM_NORMAL);
	Menu6->Append(MenuCameraMoveBackward);
	Menu6->AppendSeparator();
	MenuCameraTiltLeft = new wxMenuItem(Menu6, ID_CAMERATILTLEFT, _("Tilt Left\tCtrl+Left"), wxEmptyString, wxITEM_NORMAL);
	Menu6->Append(MenuCameraTiltLeft);
	MenuCameraTiltRight = new wxMenuItem(Menu6, ID_CAMERATILTRIGHT, _("Tilt Right\tCtrl+Right"), wxEmptyString, wxITEM_NORMAL);
	Menu6->Append(MenuCameraTiltRight);
	MenuCameraTiltUp = new wxMenuItem(Menu6, ID_CAMERATILTUP, _("Tilt Up\tCtrl+Up"), wxEmptyString, wxITEM_NORMAL);
	Menu6->Append(MenuCameraTiltUp);
	MenuCameraTiltDown = new wxMenuItem(Menu6, ID_CAMERATILTDOWN, _("Tilt Down\tCTRL+Down"), wxEmptyString, wxITEM_NORMAL);
	Menu6->Append(MenuCameraTiltDown);
	MenuBar1->Append(Menu6, _("&Camera"));
	Menu2 = new wxMenu();
	MenuHelpAbout = new wxMenuItem(Menu2, wxID_ABOUT, _("&About\tF12"), _("About this application"), wxITEM_NORMAL);
	Menu2->Append(MenuHelpAbout);
	MenuBar1->Append(Menu2, _("&Help"));
	SetMenuBar(MenuBar1);
	StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
	int __wxStatusBarWidths_1[3] = { -10, -10, -10 };
	int __wxStatusBarStyles_1[3] = { wxSB_NORMAL, wxSB_NORMAL, wxSB_NORMAL };
	StatusBar1->SetFieldsCount(3,__wxStatusBarWidths_1);
	StatusBar1->SetStatusStyles(3,__wxStatusBarStyles_1);
	SetStatusBar(StatusBar1);

	Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&OpenGLWindow::OnTimer1Trigger);
	Connect(wxID_EXIT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenGLWindow::OnMenuFileQuitSelected);
	Connect(ID_VIEWFULLSCREEN,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenGLWindow::OnMenuViewFullScreenSelected);
	Connect(ID_VIEWSHOWXYPLANE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenGLWindow::OnMenuViewShowXYPlaneSelected);
	Connect(ID_VIEWMODEL1,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenGLWindow::OnMenuViewModel1Selected);
	Connect(ID_VIEWMODEL2,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenGLWindow::OnMenuViewModel2Selected);
	Connect(ID_VIEWMODEL3,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenGLWindow::OnMenuViewModel3Selected);
	Connect(ID_MODELAUTOROTATION,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenGLWindow::OnMenuModelAutoRotationSelected);
	Connect(ID_MODELROTATEUP,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenGLWindow::OnMenuModelRotateUpSelected);
	Connect(ID_MODELROTATEDOWN,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenGLWindow::OnMenuModelRotateDownSelected);
	Connect(ID_CAMERALOOKATORIGIN,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenGLWindow::OnMenuCameraLookAtOriginSelected);
	Connect(ID_CAMERAZOOMIN,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenGLWindow::OnMenuCameraZoomInSelected);
	Connect(ID_CAMERAZOOMOUT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenGLWindow::OnMenuCameraZoomOutSelected);
	Connect(ID_CAMERAMOVELEFT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenGLWindow::OnMenuCameraMoveLeftSelected);
	Connect(ID_CAMERAMOVERIGHT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenGLWindow::OnMenuCameraMoveRightSelected);
	Connect(ID_CAMERAMOVEUP,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenGLWindow::OnMenuCameraMoveUpSelected);
	Connect(ID_CAMERAMOVEDOWN,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenGLWindow::OnMenuCameraMoveDownSelected);
	Connect(ID_CAMERAMOVEFORWARD,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenGLWindow::OnMenuCameraMoveForwardSelected);
	Connect(ID_CAMERAMOVEBACKWARD,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenGLWindow::OnMenuCameraMoveBackwardSelected);
	Connect(ID_CAMERATILTLEFT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenGLWindow::OnMenuCameraTiltLeftSelected);
	Connect(ID_CAMERATILTRIGHT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenGLWindow::OnMenuCameraTiltRightSelected);
	Connect(ID_CAMERATILTUP,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenGLWindow::OnMenuCameraTiltUpSelected);
	Connect(ID_CAMERATILTDOWN,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenGLWindow::OnMenuCameraTiltDownSelected);
	Connect(wxID_ABOUT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&OpenGLWindow::OnMenuHelpAboutSelected);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&OpenGLWindow::OnClose);
	Connect(wxEVT_SIZE,(wxObjectEventFunction)&OpenGLWindow::OnResize);
	//*)
}

OpenGLWindow::~OpenGLWindow()
{
	//(*Destroy(OpenGLWindow)
	//*)
}

void OpenGLWindow::OnTimer1Trigger(wxTimerEvent& event)
{
}

void OpenGLWindow::OnResize(wxSizeEvent& event)
{
}

void OpenGLWindow::OnClose(wxCloseEvent& event)
{
}

void OpenGLWindow::OnMenuFileQuitSelected(wxCommandEvent& event)
{
}

void OpenGLWindow::OnMenuHelpAboutSelected(wxCommandEvent& event)
{
}

void OpenGLWindow::OnMenuViewFullScreenSelected(wxCommandEvent& event)
{
}

void OpenGLWindow::OnMenuViewShowXYPlaneSelected(wxCommandEvent& event)
{
}

void OpenGLWindow::OnMenuViewModel1Selected(wxCommandEvent& event)
{
}

void OpenGLWindow::OnMenuViewModel2Selected(wxCommandEvent& event)
{
}

void OpenGLWindow::OnMenuViewModel3Selected(wxCommandEvent& event)
{
}


void OpenGLWindow::OnMenuModelAutoRotationSelected(wxCommandEvent& event)
{
}

void OpenGLWindow::OnMenuModelRotateUpSelected(wxCommandEvent& event)
{
}

void OpenGLWindow::OnMenuModelRotateDownSelected(wxCommandEvent& event)
{
}

void OpenGLWindow::OnMenuCameraLookAtOriginSelected(wxCommandEvent& event)
{
}

void OpenGLWindow::OnMenuCameraZoomInSelected(wxCommandEvent& event)
{
}

void OpenGLWindow::OnMenuCameraZoomOutSelected(wxCommandEvent& event)
{
}

void OpenGLWindow::OnMenuCameraMoveLeftSelected(wxCommandEvent& event)
{
}

void OpenGLWindow::OnMenuCameraMoveRightSelected(wxCommandEvent& event)
{
}

void OpenGLWindow::OnMenuCameraMoveUpSelected(wxCommandEvent& event)
{
}

void OpenGLWindow::OnMenuCameraMoveDownSelected(wxCommandEvent& event)
{
}

void OpenGLWindow::OnMenuCameraMoveForwardSelected(wxCommandEvent& event)
{
}

void OpenGLWindow::OnMenuCameraMoveBackwardSelected(wxCommandEvent& event)
{
}

void OpenGLWindow::OnMenuCameraTiltLeftSelected(wxCommandEvent& event)
{
}

void OpenGLWindow::OnMenuCameraTiltRightSelected(wxCommandEvent& event)
{
}

void OpenGLWindow::OnMenuCameraTiltUpSelected(wxCommandEvent& event)
{
}

void OpenGLWindow::OnMenuCameraTiltDownSelected(wxCommandEvent& event)
{
}
            