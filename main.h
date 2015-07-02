#ifndef LIFE3DMAIN_H
#define LIFE3DMAIN_H

#include <wx/wx.h>

class life3dFrame: public wxFrame
{
    public:
        life3dFrame(wxFrame *frame, const wxString &title);
        ~life3dFrame();
    private:
        static const int id_quit = 1000;
        int speed;
        void OnClose(wxCloseEvent &event);
        void OnQuit(wxCommandEvent &event);
        void OnAbout(wxCommandEvent &event);
        void OnHelp(wxCommandEvent &event);
        DECLARE_EVENT_TABLE()
};


#endif
