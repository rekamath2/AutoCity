#include <wx/app.h>
#include <wx/msgdlg.h>			// for wxMessageBox()
#include "MainWindow.h"

class myApp : public wxApp
{
public:
    virtual bool OnInit()
    {
        MainWindow *mw = new MainWindow();//Create Main Window
        if (mw)
        {
            mw->Show(true);
            if (!(mw->initializeGL()))//Initialize OpenGL for Main Window, check for errors
            {
                wxMessageBox(_("InnitializeGL( ) wsa failed!"),
                         _("Error"), wxICON_ERROR);
                return false;
            }


            // Return back to wxWidget system
            // and wait for any event to occurs
            return true;
        }
        else
        {
            wxMessageBox(_("Window is not created!"),
                         _("Error"), wxICON_ERROR);
            return false;
        }
    }
};

IMPLEMENT_APP(myApp);
