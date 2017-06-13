#include "acGLFrame.h"

#include <wx/string.h>
#include <wx/intl.h>

const long acGLFrame::ID_GLCANVAS = wxNewId();
const long acGLFrame::ID_TIMER1 = wxNewId();

BEGIN_EVENT_TABLE(acGLFrame,wxFrame)
END_EVENT_TABLE()

acGLFrame::acGLFrame(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
{
	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);

	Timer1.SetOwner(this, ID_TIMER1);
	Timer1.Start(16, false);

	menuFile = new wxMenu();
	menuFile->Append(1, "&Hello\tCtrl-H",
			"Help string shown in status bar for this item");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);

	menuHelp = new wxMenu();
	menuHelp->Append(wxID_ABOUT);

	menuBar = new wxMenuBar();
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");
	SetMenuBar(menuBar);

	CreateStatusBar();
	SetStatusText("AutoCity v1.0.0");

	m_acToolBar = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY);
	m_acToolNew = m_acToolBar->AddTool( wxID_ANY, wxT("tool"),
			wxArtProvider::GetBitmap( wxART_NEW, wxART_TOOLBAR ),
			wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );
	m_acToolOpen = m_acToolBar->AddTool( wxID_ANY, wxT("tool"),
			wxArtProvider::GetBitmap( wxART_FILE_OPEN, wxART_TOOLBAR ),
			wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	m_acToolBar->AddSeparator();
	m_acToolSave = m_acToolBar->AddTool( wxID_ANY, wxT("tool"),
			wxArtProvider::GetBitmap( wxART_HARDDISK, wxART_TOOLBAR ),
			wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	m_acToolExport = m_acToolBar->AddTool( wxID_ANY, wxT("tool"),
			wxArtProvider::GetBitmap( wxART_HARDDISK, wxART_TOOLBAR ),
			wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	m_acToolUndo = m_acToolBar->AddTool( wxID_ANY, wxT("tool"),
			wxArtProvider::GetBitmap( wxART_UNDO, wxART_TOOLBAR ),
			wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	m_acToolUndo = m_acToolBar->AddTool( wxID_ANY, wxT("tool"),
			wxArtProvider::GetBitmap( wxART_REDO, wxART_TOOLBAR ),
			wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	m_acToolBar->Realize();

	int GLCanvasAttributes[] = {
		WX_GL_RGBA,
		WX_GL_DOUBLEBUFFER,
		WX_GL_DEPTH_SIZE,	16,
		WX_GL_STENCIL_SIZE,	0,
		0, 0};
	
	m_acSplitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition,
			wxDefaultSize, wxSP_3D);
	acGLCanvas = new wxGLCanvas(m_acSplitter,ID_GLCANVAS,GLCanvasAttributes,wxDefaultPosition, 
			wxDefaultSize,0,_T("ID_GLCANVAS"));
	m_acProperty = new wxPropertyGrid(m_acSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize,
			wxPG_AUTO_SORT | wxPG_SPLITTER_AUTO_CENTER | wxPG_DEFAULT_STYLE);
	
	Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&acGLFrame::OnTimer1Trigger);
	Connect(1,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&acGLFrame::OnHello);
	Connect(wxID_EXIT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&acGLFrame::OnExit);
	Connect(wxID_ABOUT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&acGLFrame::OnAbout);
	Connect(wxEVT_SIZE,(wxObjectEventFunction)&acGLFrame::OnResize);
}

acGLFrame::~acGLFrame()
{

}

void acGLFrame::OnTimer1Trigger(wxTimerEvent& event)
{
}

void acGLFrame::OnHello(wxCommandEvent& event)
{
}

void acGLFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void acGLFrame::OnAbout(wxCommandEvent& event)
{
}

void acGLFrame::OnResize(wxSizeEvent& event)
{
}

void acGLFrame::acSplitterOnIdle(wxIdleEvent& event)
{
}
