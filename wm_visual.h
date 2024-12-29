#ifndef WM_VISUAL_H
#define WM_VISUAL_H

#include <X11/Xlib.h>
#include "wm.h"

class VisualMgr {
public:
    VisualMgr();
    ~VisualMgr();

    bool init_compositor(Display *display);
    void draw_title_bar(wm_info wm, string title);

private:
    XColor title_bar_bg;
    XRenderColor title_bar_xbg;
    XRenderPictFormat *pict_format;
};

#endif // WM_VISUAL_H
