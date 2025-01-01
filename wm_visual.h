#ifndef WM_VISUAL_H
#define WM_VISUAL_H

#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/Xcomposite.h>

#define ALPHA_50 0x80
#define TITLE_BAR_H 30

void init_xcomp(Display *display);
void draw_border(Display *display, Window window, Colormap color_map);
void set_title(Display *display, Window title_bar, int screen, const std::string& title);
void draw_title_bar(Display *display, Window title_bar, XRenderPictFormat *pict_format);
XRenderPictFormat* init_xrender(Display *display, int screen);
Window create_title_bar(Display *display, Window parent_window);

// Colors
struct CyanBlue{
    int red = 10;
    int blue = 10;
    int green = 10;
};


#endif // WM_VISUAL_H