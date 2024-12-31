#include <string>
#include <iostream>
#include "wm_visual.h"
#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
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

    XRenderColor color = {0x0000, 0x7F7F, 0xFFFF, 0xFFFF};
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

void set_title(Display *display, Window title_bar, int screen, const std::string& title) {
    XWindowAttributes title_bar_attrs;
    XGetWindowAttributes(display, title_bar, &title_bar_attrs);

    XftFont *default_font = XftFontOpenName(display, screen, "Space Mono-Bold"); // Replace with your font
    if (!default_font) {
        std::cerr << "Font 'Mokoto' not found!" << std::endl;
        exit(1);
    }

    XftDraw *xft_draw = XftDrawCreate(display, title_bar, DefaultVisual(display, screen), DefaultColormap(display, screen));
    if (!xft_draw) {
        std::cerr << "Failed to create XftDraw object!" << std::endl;
        XftFontClose(display, default_font);
        exit(1);
    }

    XRenderColor render_color = {0xffff, 0xffff, 0xffff, 0xffff}; // White color
    XftColor font_color;
    if (!XftColorAllocValue(display, DefaultVisual(display, screen), DefaultColormap(display, screen), &render_color, &font_color)) {
        std::cerr << "Failed to allocate font color!" << std::endl;
        XftFontClose(display, default_font);
        XftDrawDestroy(xft_draw);
        exit(1);
    }

    XGlyphInfo extents;
    XftTextExtentsUtf8(display, default_font, (const FcChar8*)title.c_str(), title.length(), &extents);

    int text_x = (title_bar_attrs.width - extents.width) / 2;
    int text_y = (title_bar_attrs.height + extents.height) / 2;

    XftDrawStringUtf8(
        xft_draw,
        &font_color,
        default_font,
        text_x, text_y,
        (const FcChar8*)title.c_str(),
        title.length()
    );

    // Clean up resources
    XftColorFree(display, DefaultVisual(display, screen), DefaultColormap(display, screen), &font_color);
    XftFontClose(display, default_font);
    XftDrawDestroy(xft_draw);
}

