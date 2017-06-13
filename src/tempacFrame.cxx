#include "acFrame.h"

const long acFrame::ID_TIMER1 = wxNewId();

acFrame::acFrame(wxWindow* parent, wxWindowID id)
{

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));

	//MENUBAR--

	wxMenu * menuFile = new wxMenu;
	menuFile->Append(1, "&Hello\tCtrl-H", 
			"Help string shown in status bar for this menu item");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);

	wxMenu *menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);
	
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");

	SetMenuBar( menuBar );
	
	//STATUSBAR--

	CreateStatusBar();
	SetStatusText ( "AutoCity v0.0.0" );

	//TOOLBAR--
	//TODO- add toolbar functionality
	
	m_acToolBar = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY );
	m_acToolNew = m_acToolBar->AddTool( wxID_ANY, wxT("tool"), 
			wxArtProvider::GetBitmap( wxART_NEW, wxART_TOOLBAR ), 
			wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);

	m_acToolOpen = m_acToolBar->AddTool( wxID_ANY, wxT("tool"), 
			wxArtProvider::GetBitmap( wxART_FILE_OPEN, wxART_TOOLBAR ), 
			wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
		
	m_acToolBar->AddSeparator(); 
			
	m_acToolSave = m_acToolBar->AddTool( wxID_ANY, wxT("tool"), 
			wxArtProvider::GetBitmap( wxART_FLOPPY, wxART_TOOLBAR ), 
			wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
				
	m_acToolExport = m_acToolBar->AddTool( wxID_ANY, wxT("tool"), 
			wxArtProvider::GetBitmap( wxART_HARDDISK, wxART_TOOLBAR ), 
			wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
					
	m_acToolUndo = m_acToolBar->AddTool( wxID_ANY, wxT("tool"), 
			wxArtProvider::GetBitmap( wxART_UNDO, wxART_TOOLBAR ), 
			wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
						
	m_acToolRedo = m_acToolBar->AddTool( wxID_ANY, wxT("tool"), 
			wxArtProvider::GetBitmap( wxART_REDO, wxART_TOOLBAR ), 
			wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
							
	m_acToolBar->Realize();

	//SPLITTERWINDOWINITIATION--
	
	m_acSplitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition,
			wxDefaultSize, wxSP_3D);
	m_acSplitter->Connect( wxEVT_IDLE,
			wxIdleEventHandler( acFrame::acSplitterOnIdle ), NULL, this);

	//PROPERTYGRID--
	//TODO

	m_acProperty = new wxPropertyGrid(m_acSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize,
			wxPG_AUTO_SORT | wxPG_SPLITTER_AUTO_CENTER | wxPG_DEFAULT_STYLE);
	m_acProperty->Append(new wxIntProperty("Number of Houses", wxPG_LABEL, 50));
	m_acProperty->Append(new wxDirProperty("Output Folder", wxPG_LABEL, ::wxGetUserHome()));


	//WXGLCANVAS--
	//TODO	

	int GLCanvasAttributes_1[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16,
		WX_GL_STENCIL_SIZE, 0, 0, 0};

	m_acGLCanvas = new wxGLCanvas(m_acSplitter, wxID_ANY,GLCanvasAttributes_1,
			wxDefaultPosition ,wxDefaultSize, 0, _T("ID_GLCANVAS1"));

	//SPLITTERWINDOW--

	m_acSplitter->SplitVertically(m_acProperty, m_acGLCanvas, 5);

	m_randomsizer = new wxBoxSizer( wxVERTICAL );

	m_randomsizer->Add( m_acSplitter, 1, wxEXPAND, 5);

	//OTHER VITAL STUFF
	this->SetSizer( m_randomsizer );
	this->Layout();
	SetSize(800,500);
	this->Centre( wxBOTH );

	//TODO: Connect events
	Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&acFrame::OnTimer1Trigger);
}


wxBEGIN_EVENT_TABLE(acFrame, wxFrame)
	    EVT_MENU(1,   acFrame::OnHello)
	    EVT_MENU(wxID_EXIT,  acFrame::OnExit)
	    EVT_MENU(wxID_ABOUT, acFrame::OnAbout)
wxEND_EVENT_TABLE()

void acFrame::InitializeGL(){
	m_acGLContext = new wxGLContext(m_acGLCanvas);
	m_acGLCanvas->SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	m_acGLCanvas->SetCurrent(*m_acGLContext);

	glClearColor(0.1f, 0.15f, 0.15f, 1.0f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	IsResized = false;
}

void acFrame::renderGL()
{
	m_acGLCanvas->SetCurrent(*m_acGLContext);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_acGLCanvas->SwapBuffers();
}

void acFrame::resizeGL(int width, int height)
{
	m_acGLCanvas->SetSize(width, height);
	glViewport(0, 0, width, height);
	IsResized = true;
}

void acFrame::OnTimer1Trigger(wxTimerEvent& event)
{
	if(!IsResized)
	{
		resizeGL(this->GetClientSize().GetWidth() / 2,
			 this->GetClientSize().GetHeight());
		//TODO: Use sash position for width
	}
	renderGL();
}

void acFrame::OnExit(wxCommandEvent& event)
{
	Close( true );
}

void acFrame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox( "This is AutoCity v0.0",
		       	"About AutoCity", wxOK | wxICON_INFORMATION);
}

void acFrame::OnHello(wxCommandEvent& event)
{
	wxMessageBox("Hello world from wxWidgets");
}

void acFrame::OnResize(wxSizeEvent& event)
{
	IsResized = false;
}
