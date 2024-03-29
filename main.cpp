#include "main.h"
#include <climits>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
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
    int *args = new int[5] {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
    glcanvas *canvas = new glcanvas(this, args);
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

glcanvas::glcanvas(life3dFrame *parent, int *args)
    : wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE, wxEmptyString), glinited(false), view_range(400), timer(this, TimerID)
{
    glRC = new wxGLContext(this);
    timer.Start(1);
    /*center = DPoint(0, -50, 0);
    direct = DPoint(0, 1, 0);
    head = DPoint(0, 0, 1);*/
    center = DPoint(0, 0, 50);
    direct = DPoint(0, 0, -1);
    head = DPoint(0, 1, 0);
    a = new Board;
    SetCursor(wxCursor(wxCURSOR_BLANK));
}

glcanvas::~glcanvas()
{
    delete glRC;
    delete a;
}

void glcanvas::OnTimer(wxTimerEvent &event)
{
    Refresh();
}

void glcanvas::OnKey(wxKeyEvent &event)
{
    int key = event.GetKeyCode();
    life3dFrame *parent = (life3dFrame *)GetParent();
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
    else if (key == 'w' || key == 's')
    {
        double ft = key == 'w' ? 1 : -1;
        ft *= view_range / 1000;
        direct = direct / length(direct);
        center = center + direct * ft;
    }
    else if (key == 'a' || key == 'd')
    {
        double ft = key == 'a' ? 1 : -1;
        ft *= view_range / 1000;
        DPoint v = cross(head, direct);
        center = center + v / length(v) * ft;
    }
    else if (key == 'q' || key == 'z')
    {
        double ft = key == 'q' ? 1 : -1;
        ft *= view_range / 1000;
        head = head / length(head);
        center = center + head * ft;
    }
    else
        event.Skip();
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
        view_range = std::max(view_range / 1.1, 1.0);
        --wheel_lines;
    }
    glResize();
}

void glcanvas::OnMouseMove(wxMouseEvent &event)
{
    wxPoint dragS(clientsize.x / 2, clientsize.y / 2);
    wxPoint dragT(event.GetX(), event.GetY());
    if (dragT == dragS)
        return;
    DPoint p;
    p.x = double(dragT.x - dragS.x) / clientsize.x;
    p.y = double(dragS.y - dragT.y) / clientsize.y;
    DPoint v = cross(p, DPoint(0, 0, -1));
    if (dcmp(length(v)))
    {
        double angle = asin(length(p) / sqrt(2)) * 2;
        DPoint newdir = rotate(DPoint(0, 0, -1), v, -angle);
        DPoint newhead = rotate(DPoint(0, 1, 0), v, -angle);
        DPoint Uy = head, Uz = direct * -1, Ux = cross(Uy, Uz);
        direct = Ux * newdir.x + Uy * newdir.y + Uz * newdir.z;
        head = Ux * newhead.x + Uy * newhead.y + Uz * newhead.z;
    }
    WarpPointer(clientsize.x / 2, clientsize.y / 2);
    Refresh();
}

void glcanvas::OnRightDown(wxMouseEvent &event)
{
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
    gluLookAt(center.x, center.y, center.z, center.x + direct.x, center.y + direct.y, center.z + direct.z, head.x, head.y, head.z);
    /*static double x = 0;
    x+=1;
    glRotated(x, 0.2, 1, 0);
    glColor3f(1, 0, 0);
    glBegin(GL_QUADS);
    glVertex3d(0.9, 0.9, 0.9);
    glVertex3d(0.9, -0.9, 0.9);
    glVertex3d(-0.9, -0.9, 0.9);
    glVertex3d(-0.9, 0.9, 0.9);
    glEnd();
    glColor3f(0, 1, 0);
    glBegin(GL_QUADS);
    glVertex3d(-0.9, 0.9, -0.9);
    glVertex3d(-0.9, -0.9, -0.9);
    glVertex3d(0.9, -0.9, -0.9);
    glVertex3d(0.9, 0.9, -0.9);
    glEnd();*/
    showLife();
    a->run();
    glFlush();
    SwapBuffers();
}
int rand(int l,int r){return rand()%(r-l+1)+l;}
void glcanvas::showLife()
{
    //glColor4f(1, 1, 1, 1);
    //showRec(DPoint(30, -30, 30), DPoint(30, -30, -30), DPoint(-30, -30, -30), DPoint(-30, -30, 30));
    static const double bsize = 0.45;
    auto data = a->getData();
    for (auto j = data->pos.begin(); j != data->pos.end(); ++j)
        for (auto i = data->a[*j].begin(); i != data->a[*j].end(); ++i)
        {
            showRec(i->x, i->y, i->z);
            /*double x = i->x, y = i->y, z = i->z;
            GLubyte ambientLight[4];
            ambientLight[3] = 200;
            ambientLight[0] = 0x66, ambientLight[1] = 0xcc, ambientLight[2] = 0xff;
            ambientLight[0] += labs(i->x % 4 - 2) * 10, ambientLight[1] += labs(i->y % 4 - 2) * 10, ambientLight[2] += labs(i->z % 4 - 2) * 10;
            glColor4ubv(ambientLight);
            //glMaterialfv(GL_FRONT, GL_AMBIENT, ambientLight);
            showRec(DPoint(x + bsize, y + bsize, z + bsize), DPoint(x + bsize, y - bsize, z + bsize), DPoint(x - bsize, y - bsize, z + bsize), DPoint(x - bsize, y + bsize, z + bsize));
            //ambientLight[0] = 0, ambientLight[1] = 1, ambientLight[2] = 0;
            glColor4ubv(ambientLight);
            //glMaterialfv(GL_FRONT, GL_AMBIENT, ambientLight);
            showRec(DPoint(x - bsize, y + bsize, z - bsize), DPoint(x - bsize, y - bsize, z - bsize), DPoint(x + bsize, y - bsize, z - bsize), DPoint(x + bsize, y + bsize, z - bsize));
            //ambientLight[0] = 0, ambientLight[1] = 0, ambientLight[2] = 1;
            glColor4ubv(ambientLight);
            //glMaterialfv(GL_FRONT, GL_AMBIENT, ambientLight);
            showRec(DPoint(x + bsize, y + bsize, z + bsize), DPoint(x + bsize, y + bsize, z - bsize), DPoint(x + bsize, y - bsize, z - bsize), DPoint(x + bsize, y - bsize, z + bsize));
            //ambientLight[0] = 1, ambientLight[1] = 1, ambientLight[2] = 0;
            glColor4ubv(ambientLight);
            //glMaterialfv(GL_FRONT, GL_AMBIENT, ambientLight);
            showRec(DPoint(x - bsize, y - bsize, z + bsize), DPoint(x - bsize, y - bsize, z - bsize), DPoint(x - bsize, y + bsize, z - bsize), DPoint(x - bsize, y + bsize, z + bsize));
            //ambientLight[0] = 1, ambientLight[1] = 0, ambientLight[2] = 1;
            glColor4ubv(ambientLight);
            //glMaterialfv(GL_FRONT, GL_AMBIENT, ambientLight);
            showRec(DPoint(x + bsize, y + bsize, z + bsize), DPoint(x + bsize, y + bsize, z - bsize), DPoint(x - bsize, y + bsize, z - bsize), DPoint(x - bsize, y + bsize, z + bsize));
            //ambientLight[0] = 0, ambientLight[1] = 1, ambientLight[2] = 1;
            glColor4ubv(ambientLight);
            //glMaterialfv(GL_FRONT, GL_AMBIENT, ambientLight);
            showRec(DPoint(x - bsize, y - bsize, z + bsize), DPoint(x - bsize, y - bsize, z - bsize), DPoint(x + bsize, y - bsize, z - bsize), DPoint(x + bsize, y - bsize, z + bsize));*/
        }
}

/*void glcanvas::showRec(const DPoint &p1, const DPoint &p2, const DPoint &p3, const DPoint &p4)
{
    glBegin(GL_QUADS);
    glVertex3d(p1.x, p1.y, p1.z);
    glVertex3d(p2.x, p2.y, p2.z);
    glVertex3d(p3.x, p3.y, p3.z);
    glVertex3d(p4.x, p4.y, p4.z);
    glEnd();
    glColor4f(1, 0, 0, 1);
    glBegin(GL_LINE_LOOP);
    glVertex3d(p1.x, p1.y, p1.z);
    glVertex3d(p2.x, p2.y, p2.z);
    glVertex3d(p3.x, p3.y, p3.z);
    glVertex3d(p4.x, p4.y, p4.z);
    glEnd();
}*/

void glcanvas::showRec(double x, double y, double z)
{
    static int lst0 = -1, lst = -1;
    if (lst == -1)
    {
        double sz = 0.4;
        lst0 = 1;
        glNewList(lst0, GL_COMPILE);
        glBegin(GL_QUADS);
        glVertex3d(sz, sz, sz);
        glVertex3d(sz, -sz, sz);
        glVertex3d(-sz, -sz, sz);
        glVertex3d(-sz, sz, sz);
        glEnd();
        glEndList();
        lst = 2;
        glNewList(lst, GL_COMPILE);
        
        glCallList(lst0);

        glPushMatrix();
        glRotated(180, 1, 0, 0);
        glCallList(lst0);
        glPopMatrix();

        glPushMatrix();
        glRotated(90, 1, 0, 0);
        glCallList(lst0);
        glPopMatrix();

        glPushMatrix();
        glRotated(-90, 1, 0, 0);
        glCallList(lst0);
        glPopMatrix();

        glPushMatrix();
        glRotated(90, 0, 1, 0);
        glCallList(lst0);
        glPopMatrix();

        glPushMatrix();
        glRotated(-90, 0, -1, 0);
        glCallList(lst0);
        glPopMatrix();

        glEndList();
    }
    glPushMatrix();
    glTranslated(x, y, z);
    GLubyte c[4] = {82, 184, 235, 234};
    int x2 = (int)x % 30 - 15;
    int y2 = (int)y % 30 - 15;
    int z2 = (int)z % 30 - 15;
    x2 = labs(x2), y2 = labs(y2), z2 = labs(z2);
    c[0] += x2 * 20, c[1] += y2 * 20, c[2] += z2 * 20;
    c[0] %= 256*2, c[1] %= 256*2, c[2] %= 256*2;
    c[0] = labs(c[0] - 256);
    c[1] = labs(c[1] - 256);
    c[2] = labs(c[2] - 256);
    glColor4ubv(c);
    glCallList(lst);
    glPopMatrix();
}

void glcanvas::glinit()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    /*glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    float ambientLight[4] = {1, 1, 1, 1};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);*/
}

void glcanvas::glResize()
{
    double width = clientsize.x, height = clientsize.y;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, width / height, 0.1, view_range);
    Refresh();
}

void glcanvas::OnSize(wxSizeEvent &event)
{
    clientsize = GetClientSize();
    glResize();
}
