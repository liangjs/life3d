#include "app.h"
#include "main.h"

IMPLEMENT_APP(life3dApp);

bool life3dApp::OnInit()
{
    life3dFrame* frame = new life3dFrame(NULL, _("game of life 3D"));
    frame->Show();
    return true;
}
