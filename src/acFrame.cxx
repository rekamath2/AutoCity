#include "acFrame.h"

#define ID_Hello 1

acFrame::acFrame(wxWindow* parent, wxWindowID id)
{

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));

	//MENUBAR--

	wxMenu * menuFile = new wxMenu;
	menuFile->Append(ID_Hello, "&New Castle\tCtrl-H", 
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
	//TODO
	
	acToolBar = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY );
	acToolNew = acToolBar->AddTool( wxID_ANY, wxT("tool"), 
			wxArtProvider::GetBitmap( wxART_NEW, wxART_TOOLBAR ), 
			wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);

	acToolOpen = acToolBar->AddTool( wxID_ANY, wxT("tool"), 
			wxArtProvider::GetBitmap( wxART_FILE_OPEN, wxART_TOOLBAR ), 
			wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
		
	acToolBar->AddSeparator(); 
			
	acToolSave = acToolBar->AddTool( wxID_ANY, wxT("tool"), 
			wxArtProvider::GetBitmap( wxART_FLOPPY, wxART_TOOLBAR ), 
			wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
				
	acToolExport = acToolBar->AddTool( wxID_ANY, wxT("tool"), 
			wxArtProvider::GetBitmap( wxART_HARDDISK, wxART_TOOLBAR ), 
			wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
					
	acToolUndo = acToolBar->AddTool( wxID_ANY, wxT("tool"), 
			wxArtProvider::GetBitmap( wxART_UNDO, wxART_TOOLBAR ), 
			wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
						
	acToolRedo = acToolBar->AddTool( wxID_ANY, wxT("tool"), 
			wxArtProvider::GetBitmap( wxART_REDO, wxART_TOOLBAR ), 
			wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL ); 
							

	acToolBar->Realize();
	
	//SPLITTERWINDOW--

	wxBoxSizer* randomsizer;
	randomsizer = new wxBoxSizer( wxVERTICAL );

	acSplitter = new wxSplitterWindow( this, wxID_ANY, 
			wxDefaultPosition, wxDefaultSize, wxSP_3D );
	acSplitter->Connect( wxEVT_IDLE, 
			wxIdleEventHandler( acFrame::acSplitterOnIdle ), NULL, this );

	randomsizer->Add( acSplitter, 1, wxEXPAND, 5);

	//OTHER VITAL STUFF
	this->SetSizer( randomsizer );
	this->Layout();
	SetSize(800,500);
	this->Centre( wxBOTH );
}


wxBEGIN_EVENT_TABLE(acFrame, wxFrame)
	    EVT_MENU(ID_Hello,   acFrame::OnHello)
	    EVT_MENU(wxID_EXIT,  acFrame::OnExit)
	    EVT_MENU(wxID_ABOUT, acFrame::OnAbout)
wxEND_EVENT_TABLE()

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
	wxLogMessage("Hello world from wxWidgets");
}
