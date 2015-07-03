#include "main.h"
#include <climits>
#include <GL/gl.h>
#include <algorithm>

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
    SetStatusText(wxString::Format("Speed:%dx", speed), 1);
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
    EVT_MOUSEWHEEL(glcanvas::OnMouseWheel)
    EVT_MOTION(glcanvas::OnMouseMove)
    EVT_TIMER(glcanvas::TimerID, glcanvas::OnTimer)
    EVT_RIGHT_DOWN(glcanvas::OnRightDown)
END_EVENT_TABLE()

glcanvas::glcanvas(life3dFrame *parent)
    : wxGLCanvas(parent, wxID_ANY, NULL, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE, wxEmptyString), glinited(false), view_range(0.03), direct(0, 0, -1), timer(this, TimerID)
{
    glRC = new wxGLContext(this);
    timer.Start(1);
}

glcanvas::~glcanvas()
{
    delete glRC;
}

void glcanvas::OnTimer(wxTimerEvent &event)
{
    Refresh();
}

void glcanvas::OnKey(wxKeyEvent &event)
{
    int key = event.GetKeyCode();
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

void glcanvas::OnMouseWheel(wxMouseEvent &event)
{
    static double wheel_angle = 0;
    wheel_angle += event.GetWheelRotation();
    int wheel_lines = wheel_angle / event.GetWheelDelta();
    wheel_angle -= wheel_lines * event.GetWheelDelta();
    while (wheel_lines < 0)
    {
        view_range *= 1.1;
        ++wheel_lines;
    }
    while (wheel_lines > 0)
    {
        view_range = std::max(view_range / 1.1, 1e-2);
        --wheel_lines;
    }
    glResize();
}

void glcanvas::OnMouseMove(wxMouseEvent &event)
{
    if (event.Dragging())
        if (event.RightIsDown())
        {
            wxPoint dragT(event.GetX(), event.GetY());
            if (event.ControlDown())
            {
                
            }
            else
            {
                DPoint v = cross(DPoint(0, 0, -1), direct);
                double angle = acos(-direct.z / length(direct));
                DPoint p;
                p.x = (dragT.x - dragS.x) * view_range;
                p.y = (dragT.y - dragS.y) * view_range;
                center = rotate(p, v, -angle) + center;
            }
            dragS = dragT;
        }
        else if (event.LeftIsDown())
        {

        }
    Refresh();
}

void glcanvas::OnRightDown(wxMouseEvent &event)
{
    dragS.x = event.GetX(), dragS.y = event.GetY();
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
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //direct=DPoint(1,0.3,0.2);
    moveToView();
    if (view_range <= 0.05)
        showGrid();
    glFlush();
    SwapBuffers();
}

void glcanvas::moveToView()
{
    glTranslated(-center.x, -center.y, -center.z);
    double angle = acos(-direct.z / length(direct));
    DPoint v = cross(DPoint(0, 0, -1), direct);
    glRotated(angle / M_PI * 180, v.x, v.y, v.z);
}

void glcanvas::showGrid()
{
    int width = clientsize.x * view_range, height = clientsize.y * view_range;
    ++width, ++height;
    int deep = std::min(width, height);
    glPushAttrib(GL_COLOR_BUFFER_BIT);
    glColor4f(0.3, 0.3, 0.3, 0.3);
    glBegin(GL_LINES);
    for (int x = -width; x <= width; ++x)
        for (int y = -height; y <= height; ++y)
        {
            glVertex3d(x, y, -deep);
            glVertex3d(x, y, deep);
        }
    for (int x = -width; x <= width; ++x)
        for (int z = -deep; z <= deep; ++z)
        {
            glVertex3d(x, -height, z);
            glVertex3d(x, height, z);
        }
    for (int y = -height; y <= height; ++y)
        for (int z = -deep; z <= deep; ++z)
        {
            glVertex3d(-width, y, z);
            glVertex3d(width, y, z);
        }
    glEnd();
    glPopAttrib();
}

void glcanvas::glinit()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
}

void glcanvas::glResize()
{
    int width = clientsize.x, height = clientsize.y;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    int deep = std::min(width, height);
    glOrtho(-width * view_range, width * view_range, -height * view_range, height * view_range, -deep * view_range, deep * view_range);
    Refresh();
}

void glcanvas::OnSize(wxSizeEvent &event)
{
    clientsize = GetClientSize();
    glResize();
}
