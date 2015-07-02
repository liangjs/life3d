#ifndef LIFE3DMAIN_H
#define LIFE3DMAIN_H

#include <wx/wx.h>
#include <wx/glcanvas.h>

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
private:
    bool glinited;
    wxGLContext *glRC;
    void glinit();
    void OnPaint(wxPaintEvent &event);
    void OnKey(wxKeyEvent &event);
    void OnSize(wxSizeEvent &event);
    DECLARE_EVENT_TABLE()
};
#endif
