#ifndef ACGLFRAME_H
#define ACGLFRAME_H

#include <GL/glew.h>

#include <wx/glcanvas.h>
#include <wx/frame.h>
#include <wx/timer.h>
#include <wx/menu.h>
#include <wx/splitter.h>
#include <wx/toolbar.h>
#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/propgrid/propgrid.h>

class acGLFrame: public wxFrame
{
	public:
		acGLFrame(wxWindow* parent, wxWindowID id=wxID_ANY, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize);
		virtual ~acGLFrame();

		wxGLCanvas* acGLCanvas;
		wxTimer Timer1;
		
		wxSplitterWindow* m_acSplitter;
		wxPropertyGrid* m_acProperty;

		virtual void acSplitterOnIdle(wxIdleEvent& event);

		wxMenu *menuFile;
		wxMenu *menuHelp;
		wxMenuBar *menuBar;
		
		wxToolBar* m_acToolBar;
		wxToolBarToolBase* m_acToolNew;
		wxToolBarToolBase* m_acToolOpen;
		wxToolBarToolBase* m_acToolSave;
		wxToolBarToolBase* m_acToolExport;
		wxToolBarToolBase* m_acToolUndo;
		wxToolBarToolBase* m_acToolRedo;

	protected:
		static const long ID_GLCANVAS;
		static const long ID_TIMER1;

	private:
		virtual void OnTimer1Trigger(wxTimerEvent& event);
		virtual void OnHello(wxCommandEvent& event);
		virtual void OnExit(wxCommandEvent& event);
		virtual void OnAbout(wxCommandEvent& event);

		virtual void OnResize(wxSizeEvent& event);

		DECLARE_EVENT_TABLE()
};
#endif
