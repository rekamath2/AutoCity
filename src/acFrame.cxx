#include "acFrame.h"

#include <iostream>

using namespace std;

acFrame::acFrame(wxWindow* parent)
	: acGLFrame(parent)
{
	SetTitle(_("wxGLCanvas OpenGL Version Supported"));
	m_acProperty->Append(new wxIntProperty("Number of Houses", wxPG_LABEL, 50));
	m_acProperty->Append(new wxDirProperty("Output Folder", wxPG_LABEL, ::wxGetUserHome()));

	m_acSplitter->SplitVertically(m_acProperty, acGLCanvas, 700);
	m_randomsizer = new wxBoxSizer( wxVERTICAL );
	m_randomsizer->Add( m_acSplitter, 1, wxEXPAND, 5);
	this->SetSizer( m_randomsizer );
	SetSize(1000,1000);
	this->Layout();
	m_randomsizer->SetSizeHints(this);
	//this->Centre( wxBOTH );
}

acFrame::~acFrame()
{
	delete acGLContext;
}

void acFrame::OnTimer1Trigger(wxTimerEvent& event)
{
	renderGL();
}

void acFrame::renderGL()
{
	acGLCanvas->SetCurrent(*acGLContext);
	glClearColor(0.1f,0.1f,0.12f,1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	acGLCanvas->SwapBuffers();
}

void acFrame::initializeGL()
{
	acGLContext = new wxGLContext(acGLCanvas);
	acGLCanvas->SetBackgroundStyle(wxBG_STYLE_CUSTOM);
	acGLCanvas->SetCurrent(*acGLContext);
	glewInit();

	glClearColor(0.1f, 0.15f, 0.15f, 1.0f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	initLoadShader();
	initLoadVertexData();
	initLoadTexture();

}

void acFrame::initLoadShader()
{

}

void acFrame::initLoadVertexData()
{

}

void acFrame::initLoadTexture()
{

}

void acFrame::OnResize(wxSizeEvent& event)
{
	this->Layout();
}
