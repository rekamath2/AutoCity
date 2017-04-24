#ifndef ACFRAME_H
#define ACFRAME_H


#include <wx/wx.h>

#include <GL/glew.h>
#include <wx/glcanvas.h>
#include <wx/frame.h>

#include <wx/wx.h>
#include <wx/string.h>
#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/xrc/xmlres.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/toolbar.h>
#include <wx/splitter.h>
#include <wx/propgrid/propgrid.h>

class acFrame : public wxFrame
{
	public:
		acFrame(wxWindow* parent, wxWindowID id = -1);

		void acSplitterOnIdle( wxIdleEvent& )
		{
			m_acSplitter->SetSashPosition( 0 );
			m_acSplitter->Disconnect( wxEVT_IDLE, wxIdleEventHandler( acFrame::acSplitterOnIdle ), NULL, this );
		}
	protected:
		void OnHello(wxCommandEvent& event);
		void OnExit(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);

		wxToolBar* m_acToolBar;
		wxToolBarToolBase* m_acToolNew;
		wxToolBarToolBase* m_acToolOpen;
		wxToolBarToolBase* m_acToolSave;
		wxToolBarToolBase* m_acToolExport;
		wxToolBarToolBase* m_acToolUndo;
		wxToolBarToolBase* m_acToolRedo;

		wxSplitterWindow* m_acSplitter;

		wxGLCanvas* m_acGLCanvas;

		wxPropertyGrid* m_acProperty;
		wxBoxSizer* m_randomsizer;

		wxDECLARE_EVENT_TABLE();
};

#endif
