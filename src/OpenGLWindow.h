#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

//(*Headers(OpenGLWindow)
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/timer.h>
#include <wx/propgrid/propgrid.h>
//*)

class OpenGLWindow: public wxFrame
{
	public:

		OpenGLWindow(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~OpenGLWindow();

		//(*Declarations(OpenGLWindow)
		wxMenuItem* MenuCameraTiltLeft;
		wxMenuItem* MenuModelRotateUp;
		wxMenu* Menu5;
		wxMenu* Menu1;
		wxStatusBar* StatusBar1;
		wxMenuItem* MenuCameraMoveRight;
		wxMenu* Menu3;
		wxMenuItem* MenuModelAutoRotation;
		wxMenu* Menu6;
		wxMenuItem* MenuFileQuit;
		wxMenuItem* MenuCameraMoveLeft;
		wxMenuItem* MenuViewFullScreen;
		wxMenuItem* MenuModelRotateDown;
		wxTimer Timer1;
		wxMenuItem* MenuCameraTiltRight;
		wxMenuItem* MenuViewShowXYPlane;
		wxMenuItem* MenuCameraMoveUp;
		wxMenuItem* MenuCameraMoveDown;
		wxMenuItem* MenuCameraMoveBackward;
		wxMenuItem* MenuCameraZoomIn;
		wxMenuItem* MenuCameraLookAtOrigin;
		wxMenuBar* MenuBar1;
		wxMenuItem* MenuCameraTiltDown;
		wxMenuItem* MenuCameraTiltUp;
		wxMenu* Menu2;
		wxMenuItem* MenuHelpAbout;
		wxMenuItem* MenuCameraMoveForward;
		wxMenuItem* MenuCameraZoomOut;
		wxMenu* Menu4;
		wxMenuItem* MenuGenHouse;
		wxMenuItem* MenuSaveHouse;
		wxMenuItem* MenuGenBuilding;
		wxMenuItem* MenuSaveBuilding;
		//*)

	protected:

		//(*Identifiers(OpenGLWindow)
		static const long ID_TIMER1;
		static const long ID_VIEWFULLSCREEN;
		static const long ID_VIEWSHOWXYPLANE;
		static const long ID_MODELAUTOROTATION;
		static const long ID_MODELROTATEUP;
		static const long ID_MODELROTATEDOWN;
		static const long ID_CAMERALOOKATORIGIN;
		static const long ID_CAMERAZOOMIN;
		static const long ID_CAMERAZOOMOUT;
		static const long ID_CAMERAMOVELEFT;
		static const long ID_CAMERAMOVERIGHT;
		static const long ID_CAMERAMOVEUP;
		static const long ID_CAMERAMOVEDOWN;
		static const long ID_CAMERAMOVEFORWARD;
		static const long ID_CAMERAMOVEBACKWARD;
		static const long ID_CAMERATILTLEFT;
		static const long ID_CAMERATILTRIGHT;
		static const long ID_CAMERATILTUP;
		static const long ID_CAMERATILTDOWN;
		static const long ID_STATUSBAR1;
		static const long ID_GENHOUSE;
		static const long ID_SAVEHOUSE;
		static const long ID_GENBUILD;
		static const long ID_SAVEBUILD;

		static const long ID_PROPDIR;
		//*)
		wxPropertyGrid* m_acProperty;

	private:

		//(*Handlers(OpenGLWindow)
		virtual void OnTimer1Trigger(wxTimerEvent& event);
		virtual void OnResize(wxSizeEvent& event);
		virtual void OnClose(wxCloseEvent& event);
		virtual void OnMenuFileQuitSelected(wxCommandEvent& event);
		virtual void OnMenuHelpAboutSelected(wxCommandEvent& event);
		virtual void OnMenuViewFullScreenSelected(wxCommandEvent& event);
		virtual void OnMenuViewShowXYPlaneSelected(wxCommandEvent& event);
		virtual void OnMenuModelAutoRotationSelected(wxCommandEvent& event);
		virtual void OnMenuModelRotateUpSelected(wxCommandEvent& event);
		virtual void OnMenuModelRotateDownSelected(wxCommandEvent& event);
		virtual void OnMenuCameraLookAtOriginSelected(wxCommandEvent& event);
		virtual void OnMenuCameraZoomInSelected(wxCommandEvent& event);
		virtual void OnMenuCameraZoomOutSelected(wxCommandEvent& event);
		virtual void OnMenuCameraMoveLeftSelected(wxCommandEvent& event);
		virtual void OnMenuCameraMoveRightSelected(wxCommandEvent& event);
		virtual void OnMenuCameraMoveUpSelected(wxCommandEvent& event);
		virtual void OnMenuCameraMoveDownSelected(wxCommandEvent& event);
		virtual void OnMenuCameraMoveForwardSelected(wxCommandEvent& event);
		virtual void OnMenuCameraMoveBackwardSelected(wxCommandEvent& event);
		virtual void OnMenuCameraTiltLeftSelected(wxCommandEvent& event);
		virtual void OnMenuCameraTiltRightSelected(wxCommandEvent& event);
		virtual void OnMenuCameraTiltUpSelected(wxCommandEvent& event);
		virtual void OnMenuCameraTiltDownSelected(wxCommandEvent& event);

		virtual void OnMenuEditGenHouse(wxCommandEvent& event);
		virtual void OnMenuEditSaveHouse(wxCommandEvent& event);

		virtual void OnMenuEditGenBuild(wxCommandEvent& event);
		virtual void OnMenuEditSaveBuild(wxCommandEvent& event);

		virtual void OnPropertyGridChanged(wxPropertyGridEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
