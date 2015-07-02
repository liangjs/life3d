#include "main.h"
#include <climits>
#include <GL/gl.h>

BEGIN_EVENT_TABLE(life3dFrame, wxFrame)
    EVT_CLOSE(life3dFrame::OnClose)
    EVT_MENU(wxID_HELP, life3dFrame::OnHelp)
    EVT_MENU(wxID_ABOUT, life3dFrame::OnAbout)
    EVT_MENU(life3dFrame::id_quit, life3dFrame::OnQuit)
END_EVENT_TABLE()

life3dFrame::life3dFrame(wxFrame *frame, const wxString &title)
    : wxFrame(frame, wxID_ANY, title), speed(1)
{
    wxMenuBar *mbar = new wxMenuBar;
    wxMenu *fileMenu = new wxMenu, *helpMenu = new wxMenu;
    fileMenu->Append(id_quit, "&Quit\tAlt-F4", "Quit game of life 3D");
    helpMenu->Append(wxID_HELP, "&Help\tF1", "Show help");
    helpMenu->Append(wxID_ABOUT, "&About", "Show info about life3d");
    mbar->Append(fileMenu, "&File");
    mbar->Append(helpMenu, "&Help");
    SetMenuBar(mbar);
    CreateStatusBar(2);
    SetStatusText("Ready", 0);
    SetStatusText("Speed:1x", 1);
    glcanvas *canvas = new glcanvas(this);
    Center();
    Maximize();
    canvas->SetFocus();
}

void life3dFrame::OnHelp(wxCommandEvent &event)
{
}

void life3dFrame::OnAbout(wxCommandEvent &event)
{
}

void life3dFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void life3dFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void life3dFrame::fresh_speed()
{
    SetStatusText(wxString::Format("Speed:%dx", speed), 1);
    Refresh();
}

BEGIN_EVENT_TABLE(glcanvas, wxGLCanvas)
    EVT_PAINT(glcanvas::OnPaint)
    EVT_CHAR(glcanvas::OnKey)
    EVT_SIZE(glcanvas::OnSize)
END_EVENT_TABLE()

glcanvas::glcanvas(life3dFrame *parent)
    : wxGLCanvas(parent, wxID_ANY, NULL, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE, wxEmptyString), glinited(false)
{
    glRC = new wxGLContext(this);
}

void glcanvas::OnKey(wxKeyEvent &event)
{
    int key = event.GetKeyCode();
    putchar(key);
    fflush(stdout);
    life3dFrame *parent = (life3dFrame*)GetParent();
    if (key == '+')
    {
        if (parent->speed * 2LL <= INT_MAX)
        {
            parent->speed <<= 1;
            parent->fresh_speed();
        }
    }
    else if (key == '-')
    {
        if (parent->speed != 1)
        {
            parent->speed >>= 1;
            parent->fresh_speed();
        }
    }
}

void glcanvas::OnPaint(wxPaintEvent &event)
{
    wxPaintDC dc(this);
    SetCurrent(*glRC);
    if (!glinited)
    {
        glinit();
        glinited = true;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_LINES);
    glVertex2d(-0.9, 0.9);
    glVertex2d(0.9, -0.9);
    glVertex2d(0.9, 0.9);
    glVertex2d(-0.9, -0.9);
    glEnd();
    glFlush();
    SwapBuffers();
}

void glcanvas::glinit()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
}

void glcanvas::OnSize(wxSizeEvent &event)
{
    wxSize clientsize = GetClientSize();
    int width = clientsize.x, height = clientsize.y;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (width <= height)
        glOrtho(-6.0, 6.0, -6.0 * (GLfloat)height / (GLfloat)width, 6.0 * (GLfloat)height / (GLfloat)width, -10.0, 10.0);
    else
        glOrtho(-6.0 * (GLfloat)width / (GLfloat)height, 6.0 * (GLfloat)width / (GLfloat)height, -6.0, 6.0, -10.0, 10.0);
}
