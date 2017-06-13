#ifndef ACFRAME_H
#define ACFRAME_H

#include "acGLFrame.h"
#include <wx/splitter.h>
#include <wx/propgrid/propgrid.h>
#include <wx/wx.h>

class acFrame : public acGLFrame
{
	wxGLContext *acGLContext;

	public:
		acFrame(wxWindow* parent=0);
		virtual ~acFrame();

		void initializeGL();

	private:
		void OnTimer1Trigger(wxTimerEvent& event);
		void renderGL();
		void OnResize(wxSizeEvent& event);

	protected:

		void initLoadShader();
		void initLoadVertexData();
		void initLoadTexture();

		wxBoxSizer* m_randomsizer;
};

#endif
