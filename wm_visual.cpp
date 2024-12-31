#include <iostream>
#include "wm_visual.h"
#include <X11/Xlib.h>
#include <string.h>
#include <X11/extensions/Xrender.h>

using namespace std;

void init_xcomp(Display *display)
{
    int event_base, error_base;
    bool is_comptab = XCompositeQueryExtension(
        display, &event_base, &error_base);
    if (!is_comptab)
    {
        cout << "x compositor not found" << endl;
        exit(1);
    };
    cout << "x compositor found" << endl;
}

XRenderPictFormat* init_xrender(Display *display, int screen){
    XRenderPictFormat *pict_format = XRenderFindVisualFormat(display, DefaultVisual(display, screen));
    if (!pict_format){
        cout << "cannot enable transparency" << endl;
        exit(2);
    }
    else{
        cout << "visual depth found "<< endl;
    }
    return pict_format;
}

Window create_title_bar(Display *display, Window parent_window)
{
    XWindowAttributes parent_win_attrs;
    XGetWindowAttributes(display, parent_window, &parent_win_attrs);

    Window title_bar = XCreateSimpleWindow(
        display,
        parent_window, 
        0, 0,
        parent_win_attrs.width, TITLE_BAR_H,
        0, 0, 0
    );

    // define types of events allowed
    XSelectInput(display, title_bar, ButtonPressMask | ButtonReleaseMask | PointerMotionMask | ExposureMask);
    XMapWindow(display, title_bar);
    return title_bar;
}

void draw_title_bar(Display *display, Window title_bar, XRenderPictFormat *pict_format){
    XWindowAttributes parent_win_attrs;
    XGetWindowAttributes(display, title_bar, &parent_win_attrs);

    XRenderPictureAttributes pict_attrs = {};
    Picture title_bar_pict = XRenderCreatePicture(
        display,
        title_bar,
        pict_format,
        0,
        &pict_attrs
    );

    XRenderColor color = {0x0000, 0xFFFF, 0xFFFF, 0x8000};
    XRenderFillRectangle(
        display,
        PictOpSrc,
        title_bar_pict,
        &color,
        0, 0,
        parent_win_attrs.width,
        TITLE_BAR_H
    );
    XRenderFreePicture(display, title_bar_pict);
    cout << "Picture drawn" << endl;
}

void set_title(Display *display, Window title_bar, const std::string& title) {
    GC title_gc = XCreateGC(display, title_bar, 0, NULL);

    XWindowAttributes title_bar_attrs;
    XGetWindowAttributes(display, title_bar, &title_bar_attrs);

    XSetFont(display, title_gc, XLoadFont(display, "fixed"));  // Or another font

    const char* title_char = title.c_str();
    int title_len = title.length();

    XDrawString(display, title_bar, title_gc, title_bar_attrs.width / 2, 15, title_char, title_len);

    XFreeGC(display, title_gc);
}
