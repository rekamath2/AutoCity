#ifndef ACAPP_H
#define ACAPP_H

#define _WXGTK_

#include <wx/wx.h>

#include "acFrame.h"

class acApp : public wxApp
{
	public:
		virtual bool OnInit();
};
#endif
