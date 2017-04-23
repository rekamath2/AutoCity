#ifndef ACFRAME_H
#define ACFRAME_H


#include <wx/wx.h>

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
			acSplitter->SetSashPosition( 0 );
			acSplitter->Disconnect( wxEVT_IDLE, wxIdleEventHandler( acFrame::acSplitterOnIdle ), NULL, this );
		}
	protected:
		void OnHello(wxCommandEvent& event);
		void OnExit(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);

		wxToolBar* acToolBar;
		wxToolBarToolBase* acToolNew;
		wxToolBarToolBase* acToolOpen;
		wxToolBarToolBase* acToolSave;
		wxToolBarToolBase* acToolExport;
		wxToolBarToolBase* acToolUndo;
		wxToolBarToolBase* acToolRedo;

		wxSplitterWindow* acSplitter;

		wxPropertyGrid* acProperty;

		wxDECLARE_EVENT_TABLE();
};

#endif
