#ifndef LIFE3DMAIN_H
#define LIFE3DMAIN_H

#include <wx/wx.h>
#include <wx/glcanvas.h>
#include "core.h"

class glcanvas;

class life3dFrame: public wxFrame
{
public:
    life3dFrame(wxFrame *frame, const wxString &title);
    void fresh_speed();
    int speed;
private:
    static const int id_quit = 1000;
    void OnClose(wxCloseEvent &event);
    void OnQuit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);
    void OnHelp(wxCommandEvent &event);
    DECLARE_EVENT_TABLE()
};

class glcanvas: public wxGLCanvas
{
public:
    glcanvas(life3dFrame *parent);
    ~glcanvas();
private:
    static const int TimerID = 1000;
    wxSize clientsize;
    bool glinited;
    wxGLContext *glRC;
    double view_range;
    DPoint center, direct, head;
    wxPoint dragS;
    wxTimer timer;
    Board *a;
    void glResize();
    void showRec(const DPoint &p1, const DPoint &p2, const DPoint &p3, const DPoint &p4);
    void showLife();
    void glinit();
    void OnPaint(wxPaintEvent &event);
    void OnKey(wxKeyEvent &event);
    void OnSize(wxSizeEvent &event);
    void OnMouseWheel(wxMouseEvent &event);
    void OnMouseMove(wxMouseEvent &event);
    void OnRightDown(wxMouseEvent &event);
    void OnTimer(wxTimerEvent &event);
    DECLARE_EVENT_TABLE()
};
#endif
