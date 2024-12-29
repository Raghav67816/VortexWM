#include <iostream>
#include "wm.h"
#include "wm_visual.h"
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/Xcomposite.h>

using namespace std;

// Constructor
VisualMgr::VisualMgr() {
    cout << "Composition manager initialized" << endl;
}

bool VisualMgr::init_compositor(Display *display) {
    int event_base_return = 0;
    int error_base_return = 0;

    if (XCompositeQueryExtension(display, &event_base_return, &error_base_return)) {
        return true;
    } else {
        return false;
    }
}

void VisualMgr::draw_title_bar(wm_info wm, string title){
    XRenderColor title_bar_xbg;
    XColor title_bar_color;

    XWindowAttributes parent_win_attrs;
    XGetWindowAttributes(wm.display, wm.window, &parent_win_attrs);

    Pixmap title_bar = XCreatePixmap(
        wm.display,
        wm.window, 
        parent_win_attrs.width,
        40,
        DefaultDepth(wm.display, wm.window)
    );

    GC title_bar_gc = XCreateGC(wm.display, title_bar, 0, NULL);

    title_bar_color.red = 0;
    title_bar_color.green = 191;
    title_bar_color.blue = 255;

    title_bar_xbg.red = title_bar_color.red;
    title_bar_xbg.green = title_bar_color.green;
    title_bar_xbg.blue = title_bar_color.blue;
    title_bar_xbg.alpha = 0x80;

    XRenderPictureAttributes render_attributes;
    render_attributes.subwindow_mode = IncludeInferiors;
    render_attributes.graphics_exposures = False;

    pict_format = XRenderFindVisualFormat(wm.display, DefaultVisual(
        wm.display, wm.screen
    ));

    Picture title_bar_pict = XRenderCreatePicture(
        wm.display,
        wm.window, 
        pict_format, 
        CPSubwindowMode, 
        &render_attributes
    );

    XRenderFillRectangle(
        wm.display,
        0,
        title_bar_pict,
        &title_bar_xbg,
        0, 0,
        parent_win_attrs.width,
        40
    );

    XCompositeRedirectWindow(wm.display, wm.window, CompositeRedirectAutomatic);
    XCopyArea(
        wm.display,
        title_bar_pict, 
        wm.window, title_bar_gc, 
        0, 0, 
        parent_win_attrs.width,
        40,
        0, 0
    );

}

VisualMgr::~VisualMgr() {
    cout << "Composition manager destroyed" << endl;
}
