#include "acApp.h"

IMPLEMENT_APP(acApp);

bool acApp::OnInit()
{
	wxInitAllImageHandlers();
	acFrame *frame = new acFrame(0);
	frame->Show( true );
	frame->InitializeGL();
	return true;
}
